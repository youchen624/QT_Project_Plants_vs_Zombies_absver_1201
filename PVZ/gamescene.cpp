#include "gamescene.h"
#include <QDateTime>
#include <QDebug>
#include <cmath>
#include <random>

// Random number generator for zombie spawning
static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<> row_dist(0, GameScene::ROWS - 1);
static std::uniform_int_distribution<> zombie_type_dist(0, 9);

GameScene::GameScene(QWidget *parent)
    : QWidget(parent)
    , m_isRunning(false)
    , m_isPaused(false)
    , m_isAdvancedPause(false)
    , m_sunAmount(150)
    , m_selectedPlantType(PLANT_NONE)
    , m_currentWave(1)
    , m_zombiesSpawned(0)
    , m_maxZombiesPerWave(10)
    , m_currentLevel(1)
    , m_totalWaves(3)
    , m_isFlagWave(false)
    , m_isEndlessMode(false)
    , m_gameSpeedMultiplier(1.0f)
    , m_autoCollectSun(false)
    , m_gameTimer(nullptr)
    , m_spawnTimer(nullptr)
    , m_sunTimer(nullptr)
    , m_initialDelayTimer(nullptr)
    , m_lastUpdateTime(0)
    , m_shootTimer(0.0f)
    , m_selectedCard(nullptr)
    , m_pauseMenu(nullptr)
    , m_optionsMenu(nullptr)
{
    setMinimumSize(1100, 600);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus); // Enable keyboard focus
    
    // Initialize grid
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            m_grid[i][j] = nullptr;
        }
    }

    m_gameTimer = new QTimer(this);
    connect(m_gameTimer, &QTimer::timeout, this, &GameScene::gameLoop);

    m_spawnTimer = new QTimer(this);
    connect(m_spawnTimer, &QTimer::timeout, this, &GameScene::spawnZombie);

    m_sunTimer = new QTimer(this);
    connect(m_sunTimer, &QTimer::timeout, this, &GameScene::spawnSunFromSky);
    
    m_initialDelayTimer = new QTimer(this);
    m_initialDelayTimer->setSingleShot(true);
    connect(m_initialDelayTimer, &QTimer::timeout, this, &GameScene::startZombieSpawning);
    
    // Create pause menu
    m_pauseMenu = new PauseMenu(this);
    m_pauseMenu->hide();
    connect(m_pauseMenu, &PauseMenu::continueGame, this, &GameScene::onPauseMenuContinue);
    connect(m_pauseMenu, &PauseMenu::showOptions, this, &GameScene::onPauseMenuShowOptions);
    connect(m_pauseMenu, &PauseMenu::exitToMenu, this, &GameScene::onPauseMenuExit);
    
    // Create options menu
    m_optionsMenu = new OptionsMenu(this);
    m_optionsMenu->hide();
    connect(m_optionsMenu, &OptionsMenu::closeMenu, this, &GameScene::onOptionsMenuClosed);
    connect(m_optionsMenu, &OptionsMenu::autoCollectChanged, this, &GameScene::onAutoCollectChanged);
    connect(m_optionsMenu, &OptionsMenu::gameSpeedChanged, this, &GameScene::onGameSpeedChanged);
    
    setupCards();
}

GameScene::~GameScene()
{
    qDeleteAll(m_plants);
    qDeleteAll(m_zombies);
    qDeleteAll(m_projectiles);
    qDeleteAll(m_suns);
    qDeleteAll(m_skeletons);
    qDeleteAll(m_plantCards);
}

void GameScene::startGame(int levelId)
{
    m_currentLevel = levelId;
    m_isEndlessMode = (levelId == 999); // Endless mode has ID 999
    resetGame();
    m_isRunning = true;
    m_isPaused = false;
    m_lastUpdateTime = QDateTime::currentMSecsSinceEpoch();
    m_gameTimer->start(16); // ~60 FPS
    m_sunTimer->start(8000); // Spawn sun from sky every 8 seconds
    
    // Start zombie spawning after 20 second delay to give player development time
    m_initialDelayTimer->start(20000);
}

void GameScene::pauseGame()
{
    m_isPaused = true;
    m_gameTimer->stop();
    m_spawnTimer->stop();
    m_sunTimer->stop();
    m_initialDelayTimer->stop();
}

void GameScene::resumeGame()
{
    m_isPaused = false;
    m_lastUpdateTime = QDateTime::currentMSecsSinceEpoch();
    m_gameTimer->start(16);
    
    // Only start spawn timer if we're past initial delay
    if (!m_initialDelayTimer->isActive()) {
        m_spawnTimer->start(3000);
    }
    m_sunTimer->start(8000);
}

void GameScene::resetGame()
{
    // Clear all entities
    qDeleteAll(m_plants);
    m_plants.clear();
    qDeleteAll(m_zombies);
    m_zombies.clear();
    qDeleteAll(m_projectiles);
    m_projectiles.clear();
    qDeleteAll(m_suns);
    m_suns.clear();
    qDeleteAll(m_skeletons);
    m_skeletons.clear();

    // Reset grid
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            m_grid[i][j] = nullptr;
        }
    }

    // Reset game state
    m_sunAmount = 150;
    m_selectedPlantType = PLANT_NONE;
    m_currentWave = 1;
    m_zombiesSpawned = 0;
    m_shootTimer = 0.0f;
    m_selectedCard = nullptr;
    
    // Set waves and difficulty based on level
    if (m_isEndlessMode) {
        m_totalWaves = 999999;
        m_maxZombiesPerWave = 15;
    } else {
        // Levels 1-5 have increasing waves and zombies
        switch(m_currentLevel) {
        case 1:
            m_totalWaves = 3;
            m_maxZombiesPerWave = 10;
            break;
        case 2:
            m_totalWaves = 4;
            m_maxZombiesPerWave = 12;
            break;
        case 3:
            m_totalWaves = 5;
            m_maxZombiesPerWave = 15;
            break;
        case 4:
            m_totalWaves = 6;
            m_maxZombiesPerWave = 18;
            break;
        case 5:
            m_totalWaves = 7;
            m_maxZombiesPerWave = 20;
            break;
        default:
            m_totalWaves = 3;
            m_maxZombiesPerWave = 10;
        }
    }
    
    m_isFlagWave = false;
    
    // Reset card selections
    for (PlantCard *card : m_plantCards) {
        card->setSelected(false);
    }

    emit sunAmountChanged(m_sunAmount);
}

void GameScene::addSun(int amount)
{
    m_sunAmount += amount;
    emit sunAmountChanged(m_sunAmount);
}

bool GameScene::spendSun(int amount)
{
    if (m_sunAmount >= amount) {
        m_sunAmount -= amount;
        emit sunAmountChanged(m_sunAmount);
        return true;
    }
    return false;
}

void GameScene::setSelectedPlants(const QVector<PlantCard::PlantType> &plants)
{
    m_selectedPlantTypes = plants;
    
    // Rebuild plant cards with selected plants
    qDeleteAll(m_plantCards);
    m_plantCards.clear();
    setupCards();
}

void GameScene::selectPlantType(PlantType type)
{
    m_selectedPlantType = type;
}

void GameScene::gameLoop()
{
    if (!m_isRunning || m_isPaused) return;

    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    float deltaTime = (currentTime - m_lastUpdateTime) / 1000.0f;
    m_lastUpdateTime = currentTime;

    // Apply game speed multiplier for both entities and card cooldowns
    float adjustedDeltaTime = deltaTime * m_gameSpeedMultiplier;
    
    updateEntities(deltaTime);
    updateCards(adjustedDeltaTime); // Use adjusted time for cooldowns
    checkCollisions();
    checkWinLoseConditions();
    shootProjectiles();

    update(); // Trigger repaint
}

void GameScene::updateEntities(float deltaTime)
{
    // Apply game speed multiplier
    float adjustedDeltaTime = deltaTime * m_gameSpeedMultiplier;
    
    // Update plants
    for (Plant *plant : m_plants) {
        if (plant) {
            plant->update(adjustedDeltaTime);
        }
    }

    // Update zombies (skip in advanced pause mode)
    if (!m_isAdvancedPause) {
        for (Zombie *zombie : m_zombies) {
            if (zombie) {
                zombie->update(adjustedDeltaTime);
            }
        }
    }

    // Update projectiles
    for (Projectile *projectile : m_projectiles) {
        if (projectile) {
            projectile->update(adjustedDeltaTime);
            // Mark projectile for deletion if it goes off screen
            if (projectile->getPosition().x() > width()) {
                projectile->markForDeletion();
            }
        }
    }

    // Update suns
    for (Sun *sun : m_suns) {
        if (sun) {
            sun->update(adjustedDeltaTime);
            
            // Auto-collect if enabled
            if (m_autoCollectSun && sun->isCollectable() && !sun->isExpired()) {
                addSun(sun->getValue());
                sun->collect();
            }
        }
    }
    
    // Update skeletons (skip in advanced pause mode)
    if (!m_isAdvancedPause) {
        for (Skeleton *skeleton : m_skeletons) {
            if (skeleton) {
                skeleton->update(deltaTime);
                // Mark skeleton for deletion if it goes off screen
                if (skeleton->getPosition().x() > width() || skeleton->getPosition().x() < 0) {
                    skeleton->markForDeletion();
                }
            }
        }
    }

    // Remove deleted entities
    m_plants.erase(std::remove_if(m_plants.begin(), m_plants.end(),
        [this](Plant *p) {
            if (p->isMarkedForDeletion()) {
                // Verify grid coordinates are valid before accessing grid
                int row = p->getRow();
                int col = p->getCol();
                if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
                    m_grid[row][col] = nullptr;
                }
                delete p;
                return true;
            }
            return false;
        }), m_plants.end());

    m_zombies.erase(std::remove_if(m_zombies.begin(), m_zombies.end(),
        [](Zombie *z) {
            if (z->isMarkedForDeletion()) {
                delete z;
                return true;
            }
            return false;
        }), m_zombies.end());

    m_projectiles.erase(std::remove_if(m_projectiles.begin(), m_projectiles.end(),
        [](Projectile *p) {
            if (p->isMarkedForDeletion()) {
                delete p;
                return true;
            }
            return false;
        }), m_projectiles.end());

    m_suns.erase(std::remove_if(m_suns.begin(), m_suns.end(),
        [](Sun *s) {
            if (s->isExpired()) {
                delete s;
                return true;
            }
            return false;
        }), m_suns.end());
        
    m_skeletons.erase(std::remove_if(m_skeletons.begin(), m_skeletons.end(),
        [](Skeleton *s) {
            if (s->isMarkedForDeletion()) {
                delete s;
                return true;
            }
            return false;
        }), m_skeletons.end());
}

void GameScene::checkCollisions()
{
    // Skip collision checks in advanced pause mode
    if (m_isAdvancedPause) {
        return;
    }
    
    // Check projectile-zombie collisions
    for (Projectile *projectile : m_projectiles) {
        if (projectile->isMarkedForDeletion()) continue;

        for (Zombie *zombie : m_zombies) {
            if (zombie->isMarkedForDeletion()) continue;

            // Simple bounding box collision
            if (projectile->getBounds().intersects(zombie->getBounds())) {
                zombie->takeDamage(projectile->getDamage());
                
                // Apply slow effect if ice projectile
                if (projectile->isIceProjectile()) {
                    zombie->applySlow(3.0f); // Slow for 3 seconds
                }
                
                projectile->markForDeletion();
                break;
            }
        }
    }

    // Check zombie-plant collisions
    for (Zombie *zombie : m_zombies) {
        if (zombie->isMarkedForDeletion()) continue;

        bool foundPlant = false;
        for (Plant *plant : m_plants) {
            if (plant->isMarkedForDeletion()) continue;

            // Ground spike plants and mines are ignored by zombies
            if (plant->isIgnoredByZombies()) {
                // Check if zombie is on the plant
                if (zombie->getRow() == plant->getRow() &&
                    zombie->getBounds().intersects(plant->getBounds())) {
                    // Check if it's a mine - trigger explosion
                    MinePlant *mine = dynamic_cast<MinePlant*>(plant);
                    if (mine && !mine->shouldExplode()) {
                        mine->triggerExplosion();
                    }
                    // Check if it's a car zombie on ground spike - instantly kill cars
                    else {
                        CarZombie *carZombie = dynamic_cast<CarZombie*>(zombie);
                        if (carZombie) {
                            zombie->takeDamage(10000); // Instantly destroy car
                        } else {
                            // Deal continuous damage to normal zombies on ground spikes
                            zombie->takeDamage(plant->getContinuousDamage());
                        }
                    }
                }
                // Don't stop zombie movement - it passes over the spike/mine
                continue;
            }

            // Check if zombie reached plant's position
            if (zombie->getRow() == plant->getRow() &&
                zombie->getBounds().intersects(plant->getBounds())) {
                
                // Check if this is a pole vaulting zombie that can vault
                PoleVaultingZombie *vaulter = dynamic_cast<PoleVaultingZombie*>(zombie);
                if (vaulter && vaulter->canVault()) {
                    // Start vaulting animation over this plant
                    vaulter->startVault();
                    // Continue moving without attacking
                    continue;
                }
                
                zombie->setAttacking(true);

                // Update attack timer and check if ready to attack
                if (zombie->getAttackTimer() <= 0.0f) {
                    plant->takeDamage(zombie->getAttackDamage());
                    zombie->resetAttackTimer();
                    // Set timer to attack interval
                    zombie->updateAttackTimer(zombie->getAttackInterval());
                }
                foundPlant = true;
                break;
            }
        }

        if (!foundPlant) {
            zombie->setAttacking(false);
        }
    }
    
    // Check skeleton-zombie collisions
    for (Skeleton *skeleton : m_skeletons) {
        if (skeleton->isMarkedForDeletion()) continue;
        
        bool foundZombie = false;
        for (Zombie *zombie : m_zombies) {
            if (zombie->isMarkedForDeletion()) continue;
            
            // Check if skeleton reached zombie's position on same row
            if (skeleton->getRow() == zombie->getRow() &&
                skeleton->getBounds().intersects(zombie->getBounds())) {
                skeleton->setAttacking(true);
                
                // Update attack timer and check if ready to attack
                if (skeleton->getAttackTimer() <= 0.0f) {
                    zombie->takeDamage(skeleton->getAttackDamage());
                    skeleton->resetAttackTimer();
                    // Set timer to attack interval
                    skeleton->updateAttackTimer(1.5f);
                }
                foundZombie = true;
                break;
            }
        }
        
        if (!foundZombie) {
            skeleton->setAttacking(false);
        }
    }
    
    // Check zombie-skeleton collisions (zombies can eat skeletons)
    for (Zombie *zombie : m_zombies) {
        if (zombie->isMarkedForDeletion()) continue;
        
        bool foundSkeleton = false;
        for (Skeleton *skeleton : m_skeletons) {
            if (skeleton->isMarkedForDeletion()) continue;
            
            // Check if zombie reached skeleton's position on same row
            if (zombie->getRow() == skeleton->getRow() &&
                zombie->getBounds().intersects(skeleton->getBounds())) {
                zombie->setAttacking(true);
                
                // Update attack timer and check if ready to attack
                if (zombie->getAttackTimer() <= 0.0f) {
                    skeleton->takeDamage(zombie->getAttackDamage());
                    zombie->resetAttackTimer();
                    // Set timer to attack interval
                    zombie->updateAttackTimer(zombie->getAttackInterval());
                }
                foundSkeleton = true;
                break;
            }
        }
        
        // If zombie found skeleton, don't reset attacking state
        // (this is handled separately from plant collision)
    }
    
    // Check car zombie crushing plants
    for (Zombie *zombie : m_zombies) {
        if (zombie->isMarkedForDeletion()) continue;
        
        CarZombie *carZombie = dynamic_cast<CarZombie*>(zombie);
        if (carZombie && carZombie->isCrushing()) {
            // Car crushes all plants it touches
            for (Plant *plant : m_plants) {
                if (plant->isMarkedForDeletion()) continue;
                
                if (zombie->getRow() == plant->getRow() &&
                    zombie->getBounds().intersects(plant->getBounds())) {
                    // Crush the plant instantly
                    plant->takeDamage(10000); // Massive damage
                }
            }
        }
    }
}

void GameScene::checkWinLoseConditions()
{
    // Check if any zombie reached the left edge
    for (Zombie *zombie : m_zombies) {
        if (zombie->getPosition().x() < 0) {
            m_isRunning = false;
            m_gameTimer->stop();
            m_spawnTimer->stop();
            emit gameLost();
            return;
        }
    }

    // Check if current wave completed and all zombies defeated
    if (m_zombiesSpawned >= m_maxZombiesPerWave && m_zombies.isEmpty()) {
        // In endless mode, never win - just keep going
        if (m_isEndlessMode) {
            // Start next wave with increasing difficulty
            m_currentWave++;
            m_zombiesSpawned = 0;
            
            // Every 2nd wave is a flag wave with more zombies
            if (m_currentWave % 2 == 0) {
                m_isFlagWave = true;
                m_maxZombiesPerWave = 15 + (m_currentWave * 3); // Increasing difficulty
                m_spawnTimer->start(2000); // Faster spawn
            } else {
                m_isFlagWave = false;
                m_maxZombiesPerWave = 10 + (m_currentWave * 2);
                m_spawnTimer->start(3000); // Normal spawn
            }
        } else if (m_currentWave >= m_totalWaves) {
            // All waves completed - player wins (normal mode)
            m_isRunning = false;
            m_gameTimer->stop();
            m_spawnTimer->stop();
            emit gameWon();
        } else {
            // Start next wave
            m_currentWave++;
            m_zombiesSpawned = 0;
            
            // Every 2nd wave is a flag wave with more zombies
            if (m_currentWave % 2 == 0) {
                m_isFlagWave = true;
                m_maxZombiesPerWave = 15 + (m_currentWave * 3); // Increasing difficulty
                m_spawnTimer->start(2000); // Faster spawn
            } else {
                m_isFlagWave = false;
                m_maxZombiesPerWave = 10 + (m_currentWave * 2);
                m_spawnTimer->start(3000); // Normal spawn
            }
        }
    }
}

void GameScene::shootProjectiles()
{
    m_shootTimer += 0.016f;
    if (m_shootTimer < 2.0f) return; // Shoot every 2 seconds
    m_shootTimer = 0.0f;

    // Each shooter plant shoots if there's a zombie in its row
    for (Plant *plant : m_plants) {
        ShooterPlant *shooter = dynamic_cast<ShooterPlant*>(plant);
        IceShooterPlant *iceShooter = dynamic_cast<IceShooterPlant*>(plant);
        RepeaterPlant *repeater = dynamic_cast<RepeaterPlant*>(plant);
        CatGrassPlant *catGrass = dynamic_cast<CatGrassPlant*>(plant);
        SuperGatlingPeaPlant *superGatling = dynamic_cast<SuperGatlingPeaPlant*>(plant);
        
        if (!shooter && !iceShooter && !repeater && !catGrass && !superGatling) continue;

        int row = plant->getRow();
        bool zombieInRow = false;
        Zombie *nearestZombie = nullptr;

        // Check if there's a zombie in this row (or adjacent rows for super gatling, or any row for cat grass)
        for (Zombie *zombie : m_zombies) {
            if (catGrass) {
                // Cat grass can track zombies across the entire field
                if (zombie->getPosition().x() > plant->getPosition().x()) {
                    zombieInRow = true;
                    if (!nearestZombie) {
                        nearestZombie = zombie;
                    } else {
                        // Find the closest zombie by distance (not just x-coordinate)
                        int dx1 = zombie->getPosition().x() - plant->getPosition().x();
                        int dy1 = zombie->getPosition().y() - plant->getPosition().y();
                        int dist1 = dx1 * dx1 + dy1 * dy1;
                        
                        int dx2 = nearestZombie->getPosition().x() - plant->getPosition().x();
                        int dy2 = nearestZombie->getPosition().y() - plant->getPosition().y();
                        int dist2 = dx2 * dx2 + dy2 * dy2;
                        
                        if (dist1 < dist2) {
                            nearestZombie = zombie;
                        }
                    }
                }
            } else if (superGatling) {
                // Super gatling pea can shoot 3 rows (current, above, below)
                if (abs(zombie->getRow() - row) <= 1 && zombie->getPosition().x() > plant->getPosition().x()) {
                    zombieInRow = true;
                    if (!nearestZombie || zombie->getPosition().x() < nearestZombie->getPosition().x()) {
                        nearestZombie = zombie;
                    }
                }
            } else if (zombie->getRow() == row && zombie->getPosition().x() > plant->getPosition().x()) {
                zombieInRow = true;
                if (!nearestZombie || zombie->getPosition().x() < nearestZombie->getPosition().x()) {
                    nearestZombie = zombie;
                }
                break;
            }
        }

        if (zombieInRow) {
            if (superGatling) {
                // Super gatling pea shoots 15 projectiles in 3 rows (5 per row)
                for (int rowOffset = -1; rowOffset <= 1; rowOffset++) {
                    int targetRow = row + rowOffset;
                    if (targetRow < 0 || targetRow >= ROWS) continue;
                    
                    for (int i = 0; i < 5; i++) {
                        Projectile *projectile = new Projectile(20, 180.0f, Projectile::NORMAL, this);
                        projectile->setPosition(plant->getPosition() + QPoint(30 + i*3, rowOffset*CELL_SIZE));
                        projectile->setRow(targetRow);
                        m_projectiles.append(projectile);
                    }
                }
            } else if (catGrass && nearestZombie) {
                // Cat grass shoots tracking projectiles
                Projectile *projectile = new Projectile(25, 120.0f, Projectile::TRACKING, this);
                projectile->setPosition(plant->getPosition() + QPoint(30, 0));
                projectile->setRow(row);
                projectile->setTarget(nearestZombie);
                // Initialize exact positions for tracking
                projectile->setPosition(plant->getPosition() + QPoint(30, 0));
                m_projectiles.append(projectile);
            } else if (repeater) {
                // Repeater shoots two projectiles
                Projectile *projectile1 = new Projectile(20, 150.0f, Projectile::NORMAL, this);
                projectile1->setPosition(plant->getPosition() + QPoint(30, 0));
                projectile1->setRow(row);
                m_projectiles.append(projectile1);
                
                Projectile *projectile2 = new Projectile(20, 150.0f, Projectile::NORMAL, this);
                projectile2->setPosition(plant->getPosition() + QPoint(35, 0));
                projectile2->setRow(row);
                m_projectiles.append(projectile2);
            } else if (iceShooter) {
                // Ice shooter shoots ice projectile with slow effect
                Projectile *projectile = new Projectile(15, 140.0f, Projectile::ICE, this);
                projectile->setPosition(plant->getPosition() + QPoint(30, 0));
                projectile->setRow(row);
                m_projectiles.append(projectile);
            } else {
                // Regular shooter
                Projectile *projectile = new Projectile(20, 150.0f, Projectile::NORMAL, this);
                projectile->setPosition(plant->getPosition() + QPoint(30, 0));
                projectile->setRow(row);
                m_projectiles.append(projectile);
            }
        }
    }
}

void GameScene::spawnZombie()
{
    if (!m_isRunning || m_isPaused) return;
    if (m_zombiesSpawned >= m_maxZombiesPerWave) {
        m_spawnTimer->stop();
        return;
    }

    // Random row using modern C++ random
    int row = row_dist(gen);
    
    // Create different zombie types based on level and wave
    Zombie *zombie = nullptr;
    int zombieType = zombie_type_dist(gen);
    
    if (m_isFlagWave && m_zombiesSpawned == 0) {
        // First zombie of flag wave is always a flag zombie
        zombie = new FlagZombie(this);
    } else if (m_currentLevel == 1) {
        // Level 1: Only normal and fast zombies
        if (m_currentWave == 1 || zombieType < 7) {
            zombie = new NormalZombie(this);
        } else {
            zombie = new FastZombie(this);
        }
    } else if (m_currentLevel == 2) {
        // Level 2: Normal, Fast, and ConeHead
        if (zombieType < 4) {
            zombie = new NormalZombie(this);
        } else if (zombieType < 7) {
            zombie = new FastZombie(this);
        } else {
            zombie = new ConeHeadZombie(this);
        }
    } else if (m_currentLevel == 3) {
        // Level 3: Add Armored zombies
        if (zombieType < 3) {
            zombie = new NormalZombie(this);
        } else if (zombieType < 5) {
            zombie = new FastZombie(this);
        } else if (zombieType < 7) {
            zombie = new ConeHeadZombie(this);
        } else if (zombieType < 9) {
            zombie = new ArmoredZombie(this);
        } else {
            zombie = new PoleVaultingZombie(this);
        }
    } else if (m_currentLevel == 4) {
        // Level 4: More tough zombies
        if (zombieType < 2) {
            zombie = new NormalZombie(this);
        } else if (zombieType < 4) {
            zombie = new FastZombie(this);
        } else if (zombieType < 6) {
            zombie = new ConeHeadZombie(this);
        } else if (zombieType < 7) {
            zombie = new PoleVaultingZombie(this);
        } else if (zombieType < 9) {
            zombie = new ArmoredZombie(this);
        } else {
            zombie = new CarZombie(this);
        }
    } else {
        // Level 5 and Endless: All zombie types with higher tough zombie rate
        if (zombieType < 1) {
            zombie = new NormalZombie(this);
        } else if (zombieType < 3) {
            zombie = new FastZombie(this);
        } else if (zombieType < 5) {
            zombie = new ConeHeadZombie(this);
        } else if (zombieType < 6) {
            zombie = new PoleVaultingZombie(this);
        } else if (zombieType < 8) {
            zombie = new ArmoredZombie(this);
        } else {
            zombie = new CarZombie(this);
        }
    }

    QPoint spawnPos = gridToPixel(row, COLS - 1);
    spawnPos.setX(spawnPos.x() + 100); // Spawn off screen right
    zombie->setPosition(spawnPos);
    zombie->syncExactPosition(); // Initialize exact float position
    zombie->setRow(row);
    m_zombies.append(zombie);
    m_zombiesSpawned++;
}

void GameScene::onSunProduced(int /*amount*/)
{
    // Create a sun object at the sunflower's position
    Plant *sunProducer = qobject_cast<Plant*>(sender());
    if (sunProducer) {
        QPoint startPos = sunProducer->getPosition();
        QPoint targetPos = startPos + QPoint(0, 50); // Drop down slightly
        
        // Determine sun value based on plant type
        int sunValue = 25; // Default for regular sunflower
        TwinSunflowerPlant *twinSunflower = dynamic_cast<TwinSunflowerPlant*>(sunProducer);
        if (twinSunflower) {
            sunValue = 75; // Twin sunflower produces 3x more (75 instead of 25)
        }
        
        Sun *sun = new Sun(startPos, targetPos, false, this);
        sun->setValue(sunValue);
        m_suns.append(sun);
    }
}

void GameScene::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw background
    painter.fillRect(rect(), QColor(135, 206, 235)); // Sky blue

    // Draw grid
    drawGrid(&painter);

    // Draw entities
    for (Plant *plant : m_plants) {
        plant->render(&painter);
    }

    for (Zombie *zombie : m_zombies) {
        zombie->render(&painter);
    }
    
    // Draw skeletons
    for (Skeleton *skeleton : m_skeletons) {
        skeleton->render(&painter);
    }

    for (Projectile *projectile : m_projectiles) {
        projectile->render(&painter);
    }

    // Draw suns
    for (Sun *sun : m_suns) {
        sun->render(&painter);
    }

    // Draw HUD
    drawHUD(&painter);
}

void GameScene::drawGrid(QPainter *painter)
{
    painter->setPen(QPen(Qt::darkGreen, 2));
    
    // Draw grass background for grid
    painter->fillRect(GRID_OFFSET_X, GRID_OFFSET_Y, 
                     COLS * CELL_SIZE, ROWS * CELL_SIZE, 
                     QColor(34, 139, 34));

    // Draw grid lines
    for (int i = 0; i <= ROWS; i++) {
        int y = GRID_OFFSET_Y + i * CELL_SIZE;
        painter->drawLine(GRID_OFFSET_X, y, 
                         GRID_OFFSET_X + COLS * CELL_SIZE, y);
    }

    for (int j = 0; j <= COLS; j++) {
        int x = GRID_OFFSET_X + j * CELL_SIZE;
        painter->drawLine(x, GRID_OFFSET_Y, 
                         x, GRID_OFFSET_Y + ROWS * CELL_SIZE);
    }
}

void GameScene::drawHUD(QPainter *painter)
{
    // Draw top panel background - increased height for two rows
    painter->setBrush(QColor(101, 67, 33));
    painter->setPen(Qt::NoPen);
    painter->drawRect(0, 0, width(), 160); // Increased from 130 to 160
    
    // === FIRST ROW: Sun display and wave info ===
    painter->setPen(Qt::black);
    painter->setFont(QFont("Arial", 18, QFont::Bold));
    
    // Sun icon - now has its own row - positioned at y=50 for better vertical centering
    QRadialGradient sunGradient(30, 40, 18);
    sunGradient.setColorAt(0, QColor(255, 255, 200));
    sunGradient.setColorAt(0.5, QColor(255, 255, 0));
    sunGradient.setColorAt(1, QColor(255, 200, 0));
    painter->setBrush(sunGradient);
    painter->setPen(QPen(QColor(255, 200, 0), 2));
    painter->drawEllipse(QPoint(30, 40), 18, 18);
    
    // Sun amount text in a background box - much wider now that it's on its own row
    QString sunText = QString::number(m_sunAmount);
    QFontMetrics fm(painter->font());
    int textWidth = fm.horizontalAdvance(sunText);
    int textHeight = fm.height();
    
    // Draw background box for sun amount - very wide since no cards to compete with
    int boxWidth = qMax(textWidth + 60, 150); // Minimum width of 150, with more padding
    painter->setBrush(QColor(139, 69, 19, 220)); // Slightly more opaque brown
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(52, 20, boxWidth, textHeight + 10, 5, 5);
    
    // Sun amount text - centered in the box
    painter->setPen(Qt::white);
    int textX = 52 + (boxWidth - textWidth) / 2;
    painter->drawText(textX, 48, sunText);

    // Draw wave info on the same row as sun, on the right side
    painter->setFont(QFont("Arial", 14));
    QString waveText;
    if (m_isEndlessMode) {
        // For endless mode, show "Wave: X" without total
        waveText = QString("Wave: %1 (Endless)").arg(m_currentWave);
    } else {
        waveText = QString("Wave: %1/%2").arg(m_currentWave).arg(m_totalWaves);
    }
    if (m_isFlagWave) {
        waveText += " [FLAG WAVE!]";
        painter->setPen(Qt::red);
    } else {
        painter->setPen(Qt::white);
    }
    painter->drawText(width() - 250, 45, waveText);
    
    // Draw game speed indicator
    painter->setFont(QFont("Arial", 14, QFont::Bold));
    painter->setPen(Qt::yellow);
    QString speedText = QString("Speed: %1x (Space)").arg(m_gameSpeedMultiplier, 0, 'f', 1);
    painter->drawText(width() - 250, 70, speedText);
    
    // Draw advanced pause indicator - positioned below game speed indicator
    if (m_isAdvancedPause) {
        painter->setFont(QFont("Arial", 14, QFont::Bold));
        painter->setPen(Qt::yellow);
        painter->drawText(width() - 250, 95, "ADVANCED PAUSE (Alt)");
        painter->setFont(QFont("Arial", 11));
        painter->drawText(width() - 250, 110, "Plant/Remove: ON | Zombies: Frozen");
    }
    
    // === SECOND ROW: Plant cards (handled elsewhere, positioned at y=60) ===
}

void GameScene::mousePressEvent(QMouseEvent *event)
{
    // Allow planting/shoveling in advanced pause mode
    if (!m_isRunning || (m_isPaused && !m_isAdvancedPause)) return;

    QPoint clickPos = event->pos();

    // Check if clicking on a sun to collect it
    for (Sun *sun : m_suns) {
        if (sun->isCollectable() && sun->getBounds().contains(clickPos)) {
            addSun(sun->getValue());
            sun->collect();
            return;
        }
    }

    // Right click cancels plant selection
    if (event->button() == Qt::RightButton) {
        if (m_selectedPlantType != PLANT_NONE) {
            m_selectedPlantType = PLANT_NONE;
            if (m_selectedCard) {
                m_selectedCard->setSelected(false);
                m_selectedCard = nullptr;
            }
            update();
        }
        return;
    }

    // Handle plant placement or removal
    if (m_selectedPlantType != PLANT_NONE) {
        int row, col;
        pixelToGrid(clickPos, row, col);
        
        if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
            if (m_selectedPlantType == PLANT_SHOVEL) {
                // Remove plant at this position
                if (m_grid[row][col] != nullptr) {
                    Plant *plant = m_grid[row][col];
                    plant->markForDeletion();
                    m_grid[row][col] = nullptr;
                    
                    // Deselect shovel after use
                    m_selectedPlantType = PLANT_NONE;
                    if (m_selectedCard) {
                        m_selectedCard->setSelected(false);
                        m_selectedCard = nullptr;
                    }
                    update();
                }
            } else {
                // Place plant
                placePlant(row, col);
            }
        }
    }
}

void GameScene::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        // Cancel plant selection first if any plant is selected
        if (m_selectedPlantType != PLANT_NONE) {
            m_selectedPlantType = PLANT_NONE;
            if (m_selectedCard) {
                m_selectedCard->setSelected(false);
                m_selectedCard = nullptr;
            }
            update();
        } else if (m_isRunning && !m_isPaused) {
            // Show pause menu if no plant is selected
            showPauseMenu();
        } else if (m_isRunning && m_isPaused && !m_isAdvancedPause) {
            // ESC also unpauses the game
            hidePauseMenu();
        }
    } else if (event->key() == Qt::Key_Alt) {
        // Alt key toggles advanced pause mode
        if (m_isRunning) {
            m_isAdvancedPause = !m_isAdvancedPause;
            if (m_isAdvancedPause) {
                // Enter advanced pause - pause timers but allow plant operations
                m_isPaused = true;
                m_gameTimer->stop(); // Stop game loop updates
                m_spawnTimer->stop();
                m_sunTimer->stop();
                m_initialDelayTimer->stop();
            } else {
                // Exit advanced pause - resume game
                m_isPaused = false;
                m_lastUpdateTime = QDateTime::currentMSecsSinceEpoch();
                m_gameTimer->start(16);
                
                // Only start spawn timer if we're past initial delay
                if (!m_initialDelayTimer->isActive()) {
                    m_spawnTimer->start(3000);
                }
                m_sunTimer->start(8000);
            }
            update();
        }
    } else if (event->key() == Qt::Key_Space) {
        // Space bar cycles game speed (1x, 3x, 5x)
        if (m_isRunning && !m_optionsMenu->isVisible()) {
            cycleGameSpeed();
            update();
        }
    }
}

void GameScene::showPauseMenu()
{
    // Pause the game if not already paused
    if (!m_isPaused) {
        pauseGame();
    }
    
    // Center the pause menu
    int x = (width() - m_pauseMenu->width()) / 2;
    int y = (height() - m_pauseMenu->height()) / 2;
    m_pauseMenu->move(x, y);
    m_pauseMenu->show();
    m_pauseMenu->raise(); // Bring to front
}

void GameScene::hidePauseMenu()
{
    m_pauseMenu->hide();
    // Only resume if not in advanced pause mode
    if (!m_isAdvancedPause) {
        resumeGame();
    }
}

void GameScene::onPauseMenuContinue()
{
    hidePauseMenu();
    resumeGame();
}

void GameScene::onPauseMenuShowOptions()
{
    hidePauseMenu();
    showOptionsMenu();
}

void GameScene::onPauseMenuExit()
{
    hidePauseMenu();
    m_isRunning = false;
    m_gameTimer->stop();
    m_spawnTimer->stop();
    m_sunTimer->stop();
    m_initialDelayTimer->stop();
    emit exitToMainMenu();
}

bool GameScene::canPlacePlant(int row, int col) const
{
    return m_grid[row][col] == nullptr;
}

bool GameScene::canUpgradePlant(int row, int col, PlantType upgradeType) const
{
    Plant *existingPlant = m_grid[row][col];
    if (!existingPlant) return false;
    
    // Twin Sunflower can only be placed on Sunflower
    if (upgradeType == PLANT_TWINSUNFLOWER) {
        return dynamic_cast<SunflowerPlant*>(existingPlant) != nullptr &&
               dynamic_cast<TwinSunflowerPlant*>(existingPlant) == nullptr;
    }
    
    // Super Gatling Pea can only be placed on Repeater
    if (upgradeType == PLANT_SUPERGATLINGPEA) {
        return dynamic_cast<RepeaterPlant*>(existingPlant) != nullptr &&
               dynamic_cast<SuperGatlingPeaPlant*>(existingPlant) == nullptr;
    }
    
    return false;
}

void GameScene::placePlant(int row, int col)
{
    // Check if this is an upgrade plant
    bool isUpgrade = (m_selectedPlantType == PLANT_TWINSUNFLOWER || 
                      m_selectedPlantType == PLANT_SUPERGATLINGPEA);
    
    if (isUpgrade) {
        if (!canUpgradePlant(row, col, m_selectedPlantType)) return;
    } else {
        if (!canPlacePlant(row, col)) return;
    }

    Plant *plant = nullptr;
    int cost = 0;

    switch (m_selectedPlantType) {
    case PLANT_SUNFLOWER:
        cost = 50;
        if (spendSun(cost)) {
            plant = new SunflowerPlant(this);
            connect(plant, SIGNAL(sunProduced(int)), this, SLOT(onSunProduced(int)));
        }
        break;
    case PLANT_SHOOTER:
        cost = 100;
        if (spendSun(cost)) {
            plant = new ShooterPlant(this);
        }
        break;
    case PLANT_WALL:
        cost = 50;
        if (spendSun(cost)) {
            plant = new WallPlant(this);
        }
        break;
    case PLANT_ICESHOOTER:
        cost = 175;
        if (spendSun(cost)) {
            plant = new IceShooterPlant(this);
        }
        break;
    case PLANT_REPEATER:
        cost = 200;
        if (spendSun(cost)) {
            plant = new RepeaterPlant(this);
        }
        break;
    case PLANT_NUCLEARBOMB:
        cost = 150;
        if (spendSun(cost)) {
            NuclearBombPlant *bomb = new NuclearBombPlant(this);
            connect(bomb, &NuclearBombPlant::explode, this, &GameScene::onNuclearExplosion);
            plant = bomb;
        }
        break;
    case PLANT_SKELETONTREE:
        cost = 350;
        if (spendSun(cost)) {
            SkeletonTreePlant *skeletonTree = new SkeletonTreePlant(this);
            connect(skeletonTree, &SkeletonTreePlant::summonSkeleton, this, &GameScene::onSkeletonSummon);
            plant = skeletonTree;
        }
        break;
    case PLANT_GROUNDSPIKE:
        cost = 100;
        if (spendSun(cost)) {
            plant = new GroundSpikePlant(this);
        }
        break;
    case PLANT_CHILIPEPPER:
        cost = 125;
        if (spendSun(cost)) {
            ChiliPepperPlant *chili = new ChiliPepperPlant(this);
            connect(chili, &ChiliPepperPlant::burnRow, this, &GameScene::onChiliBurn);
            plant = chili;
        }
        break;
    case PLANT_MINE:
        cost = 25;
        if (spendSun(cost)) {
            MinePlant *mine = new MinePlant(this);
            connect(mine, &MinePlant::explode, this, &GameScene::onMineExplode);
            plant = mine;
        }
        break;
    case PLANT_TWINSUNFLOWER:
        cost = 150;
        if (spendSun(cost) && canUpgradePlant(row, col, PLANT_TWINSUNFLOWER)) {
            // Remove old sunflower
            Plant *oldPlant = m_grid[row][col];
            m_plants.removeOne(oldPlant);
            oldPlant->deleteLater();
            m_grid[row][col] = nullptr;
            
            // Place twin sunflower
            TwinSunflowerPlant *twinSunflower = new TwinSunflowerPlant(this);
            connect(twinSunflower, &TwinSunflowerPlant::sunProduced, this, &GameScene::onSunProduced);
            plant = twinSunflower;
        }
        break;
    case PLANT_CATGRASS:
        cost = 225;
        if (spendSun(cost)) {
            plant = new CatGrassPlant(this);
        }
        break;
    case PLANT_SUPERGATLINGPEA:
        cost = 800;
        if (spendSun(cost) && canUpgradePlant(row, col, PLANT_SUPERGATLINGPEA)) {
            // Remove old repeater
            Plant *oldPlant = m_grid[row][col];
            m_plants.removeOne(oldPlant);
            oldPlant->deleteLater();
            m_grid[row][col] = nullptr;
            
            // Place super gatling pea
            plant = new SuperGatlingPeaPlant(this);
        }
        break;
    case PLANT_SUPERNUCLEARBOMB:
        cost = 1500;
        if (spendSun(cost)) {
            SuperNuclearBombPlant *superBomb = new SuperNuclearBombPlant(this);
            connect(superBomb, &SuperNuclearBombPlant::superExplode, this, &GameScene::onSuperNuclearExplosion);
            plant = superBomb;
        }
        break;
    default:
        return;
    }

    if (plant) {
        plant->setPosition(gridToPixel(row, col));
        plant->setRow(row);
        plant->setCol(col);
        m_plants.append(plant);
        m_grid[row][col] = plant;
        
        // Start cooldown on the card - different cooldowns for different plants
        if (m_selectedCard) {
            float cooldownTime = 5.0f;
            if (m_selectedPlantType == PLANT_NUCLEARBOMB) {
                cooldownTime = 45.0f;
            } else if (m_selectedPlantType == PLANT_CHILIPEPPER) {
                cooldownTime = 25.0f;
            } else if (m_selectedPlantType == PLANT_SKELETONTREE) {
                cooldownTime = 10.0f;
            } else if (m_selectedPlantType == PLANT_GROUNDSPIKE) {
                cooldownTime = 7.0f;
            } else if (m_selectedPlantType == PLANT_MINE) {
                cooldownTime = 7.5f;
            } else if (m_selectedPlantType == PLANT_TWINSUNFLOWER) {
                cooldownTime = 10.0f;
            } else if (m_selectedPlantType == PLANT_CATGRASS) {
                cooldownTime = 8.0f;
            } else if (m_selectedPlantType == PLANT_SUPERGATLINGPEA) {
                cooldownTime = 30.0f;
            } else if (m_selectedPlantType == PLANT_SUPERNUCLEARBOMB) {
                cooldownTime = 120.0f;
            }
            m_selectedCard->startCooldown(cooldownTime);
            m_selectedCard->setSelected(false);
        }
        
        m_selectedPlantType = PLANT_NONE;
        m_selectedCard = nullptr;
        update();
    }
}

QPoint GameScene::gridToPixel(int row, int col) const
{
    int x = GRID_OFFSET_X + col * CELL_SIZE + CELL_SIZE / 2;
    int y = GRID_OFFSET_Y + row * CELL_SIZE + CELL_SIZE / 2;
    return QPoint(x, y);
}

void GameScene::pixelToGrid(const QPoint &pixel, int &row, int &col) const
{
    col = (pixel.x() - GRID_OFFSET_X) / CELL_SIZE;
    row = (pixel.y() - GRID_OFFSET_Y) / CELL_SIZE;
}

void GameScene::setupCards()
{
    // Use selected plants if available, otherwise use default set
    QVector<PlantCard::PlantType> plantsToUse;
    if (m_selectedPlantTypes.isEmpty()) {
        // Default plant selection - includes new plants
        plantsToUse = {
            PlantCard::SUNFLOWER,
            PlantCard::SHOOTER,
            PlantCard::WALL,
            PlantCard::ICESHOOTER,
            PlantCard::REPEATER,
            PlantCard::NUCLEARBOMB,
            PlantCard::SKELETONTREE,
            PlantCard::GROUNDSPIKE,
            PlantCard::CHILIPEPPER,
            PlantCard::MINE,
            PlantCard::SHOVEL
        };
    } else {
        plantsToUse = m_selectedPlantTypes;
        // Always add shovel
        if (!plantsToUse.contains(PlantCard::SHOVEL)) {
            plantsToUse.append(PlantCard::SHOVEL);
        }
    }
    
    // Create plant cards based on selection
    for (PlantCard::PlantType plantType : plantsToUse) {
        int cost = 0;
        switch (plantType) {
        case PlantCard::SUNFLOWER:
            cost = 50;
            break;
        case PlantCard::SHOOTER:
            cost = 100;
            break;
        case PlantCard::WALL:
            cost = 50;
            break;
        case PlantCard::ICESHOOTER:
            cost = 175;
            break;
        case PlantCard::REPEATER:
            cost = 200;
            break;
        case PlantCard::NUCLEARBOMB:
            cost = 150;
            break;
        case PlantCard::SKELETONTREE:
            cost = 350;
            break;
        case PlantCard::GROUNDSPIKE:
            cost = 100;
            break;
        case PlantCard::CHILIPEPPER:
            cost = 125;
            break;
        case PlantCard::MINE:
            cost = 25;
            break;
        case PlantCard::TWINSUNFLOWER:
            cost = 150;
            break;
        case PlantCard::CATGRASS:
            cost = 225;
            break;
        case PlantCard::SUPERGATLINGPEA:
            cost = 800;
            break;
        case PlantCard::SUPERNUCLEARBOMB:
            cost = 1500;
            break;
        case PlantCard::SHOVEL:
            cost = 0;
            break;
        }
        
        PlantCard *card = new PlantCard(plantType, cost, this);
        connect(card, &PlantCard::clicked, this, &GameScene::onCardClicked);
        m_plantCards.append(card);
    }
    
    // Position cards horizontally at the top - SECOND ROW for plant cards
    int cardX = 20; // Start position from left, more space now
    int cardY = 60;  // Second row position (was 20)
    int maxWidth = width() - 40; // Leave some margin
    int currentRowWidth = 0;
    
    for (PlantCard *card : m_plantCards) {
        // Check if card would exceed screen width
        if (currentRowWidth + 90 > maxWidth && currentRowWidth > 0) {
            // Move to next row if needed (though we'll make them scroll instead)
            // For now, just continue on same row - they'll be cut off but user can scroll
        }
        card->move(cardX, cardY);
        card->show();
        cardX += 90; // Spacing between cards
        currentRowWidth += 90;
    }
    
    // Add scroll hint if cards exceed screen
    if (currentRowWidth > maxWidth) {
        qDebug() << "Plant cards exceed screen width. Total width:" << currentRowWidth << "Max:" << maxWidth;
    }
}

void GameScene::updateCards(float deltaTime)
{
    for (PlantCard *card : m_plantCards) {
        card->updateCooldown(deltaTime);
        // Shovel is always enabled, others depend on sun amount
        if (card->getPlantType() == PlantCard::SHOVEL) {
            card->setEnabled(true);
        } else {
            card->setEnabled(m_sunAmount >= card->getCost());
        }
    }
}

void GameScene::onCardClicked(PlantCard::PlantType type)
{
    // Deselect previous card
    if (m_selectedCard) {
        m_selectedCard->setSelected(false);
    }
    
    // Find and select new card
    for (PlantCard *card : m_plantCards) {
        if (card->getPlantType() == type) {
            m_selectedCard = card;
            card->setSelected(true);
            
            // Map card type to plant type
            switch (type) {
            case PlantCard::SUNFLOWER:
                m_selectedPlantType = PLANT_SUNFLOWER;
                break;
            case PlantCard::SHOOTER:
                m_selectedPlantType = PLANT_SHOOTER;
                break;
            case PlantCard::WALL:
                m_selectedPlantType = PLANT_WALL;
                break;
            case PlantCard::ICESHOOTER:
                m_selectedPlantType = PLANT_ICESHOOTER;
                break;
            case PlantCard::REPEATER:
                m_selectedPlantType = PLANT_REPEATER;
                break;
            case PlantCard::NUCLEARBOMB:
                m_selectedPlantType = PLANT_NUCLEARBOMB;
                break;
            case PlantCard::SKELETONTREE:
                m_selectedPlantType = PLANT_SKELETONTREE;
                break;
            case PlantCard::GROUNDSPIKE:
                m_selectedPlantType = PLANT_GROUNDSPIKE;
                break;
            case PlantCard::CHILIPEPPER:
                m_selectedPlantType = PLANT_CHILIPEPPER;
                break;
            case PlantCard::MINE:
                m_selectedPlantType = PLANT_MINE;
                break;
            case PlantCard::TWINSUNFLOWER:
                m_selectedPlantType = PLANT_TWINSUNFLOWER;
                break;
            case PlantCard::CATGRASS:
                m_selectedPlantType = PLANT_CATGRASS;
                break;
            case PlantCard::SUPERGATLINGPEA:
                m_selectedPlantType = PLANT_SUPERGATLINGPEA;
                break;
            case PlantCard::SUPERNUCLEARBOMB:
                m_selectedPlantType = PLANT_SUPERNUCLEARBOMB;
                break;
            case PlantCard::SHOVEL:
                m_selectedPlantType = PLANT_SHOVEL;
                break;
            default:
                break;
            }
            break;
        }
    }
    update();
}

void GameScene::spawnSunFromSky()
{
    if (!m_isRunning || m_isPaused) return;
    
    // Random position within game grid
    static std::uniform_int_distribution<> x_dist(GRID_OFFSET_X, GRID_OFFSET_X + COLS * CELL_SIZE - 50);
    static std::uniform_int_distribution<> y_dist(GRID_OFFSET_Y + 50, GRID_OFFSET_Y + ROWS * CELL_SIZE - 50);
    
    QPoint startPos(x_dist(gen), -30);
    QPoint targetPos(startPos.x(), y_dist(gen));
    
    Sun *sun = new Sun(startPos, targetPos, true, this);
    m_suns.append(sun);
}

void GameScene::startZombieSpawning()
{
    if (m_isRunning && !m_isPaused) {
        m_spawnTimer->start(3000); // Spawn zombie every 3 seconds
    }
}

void GameScene::onNuclearExplosion()
{
    // Mark all zombies for deletion (they'll be cleaned up in the next game loop iteration)
    // This is safe because we're not modifying the collection, just marking objects for deletion
    for (Zombie *zombie : m_zombies) {
        if (zombie && !zombie->isMarkedForDeletion()) {
            zombie->markForDeletion();
        }
    }
    
    qDebug() << "Nuclear explosion! All zombies destroyed!";
}

void GameScene::onSkeletonSummon(int treeRow)
{
    // Summon TWO skeletons on upper and lower adjacent lanes (not tree's current lane)
    SkeletonTreePlant *tree = qobject_cast<SkeletonTreePlant*>(sender());
    if (!tree) return;
    
    // Summon TWO skeletons on adjacent lanes (treeRow±1)
    for (int offset = -1; offset <= 1; offset += 2) { // -1 and +1 only
        int targetRow = treeRow + offset;
        if (targetRow >= 0 && targetRow < ROWS) {
            // Create a skeleton entity at the tree's position
            Skeleton *skeleton = new Skeleton(this);
            QPoint skeletonPos = gridToPixel(targetRow, tree->getCol());
            skeleton->setPosition(skeletonPos);
            skeleton->setRow(targetRow);
            skeleton->syncExactPosition();
            skeleton->setDirection(1); // Move right towards zombies
            m_skeletons.append(skeleton);
        }
    }
    
    qDebug() << "Skeleton tree summoned 2 skeletons on adjacent lanes!";
}

void GameScene::onChiliBurn(int row)
{
    // Burn all zombies in the specified row
    for (Zombie *zombie : m_zombies) {
        if (zombie && zombie->getRow() == row && !zombie->isMarkedForDeletion()) {
            zombie->markForDeletion();
        }
    }
    
    qDebug() << "Chili pepper burned all zombies in row" << row;
}

void GameScene::onMineExplode(int row, int col)
{
    // Mine explosion kills most zombies in a small area around it
    QPoint minePos = gridToPixel(row, col);
    int explosionRadius = 80; // Radius of explosion effect
    
    for (Zombie *zombie : m_zombies) {
        if (zombie && !zombie->isMarkedForDeletion()) {
            QPoint zombiePos = zombie->getPosition();
            int distance = static_cast<int>(sqrt(pow(zombiePos.x() - minePos.x(), 2) + 
                                                  pow(zombiePos.y() - minePos.y(), 2)));
            
            if (distance <= explosionRadius) {
                // Instant kill for most zombies (massive damage)
                zombie->takeDamage(1800);
            }
        }
    }
    
    qDebug() << "Mine exploded at row" << row << "col" << col;
}

void GameScene::onSuperNuclearExplosion()
{
    // Super nuclear bomb destroys EVERYTHING - all plants and zombies
    
    // Mark all zombies for deletion
    for (Zombie *zombie : m_zombies) {
        if (zombie && !zombie->isMarkedForDeletion()) {
            zombie->markForDeletion();
        }
    }
    
    // Mark all plants for deletion (except the super bomb itself which will delete itself)
    for (Plant *plant : m_plants) {
        if (plant && !plant->isMarkedForDeletion()) {
            SuperNuclearBombPlant *superBomb = dynamic_cast<SuperNuclearBombPlant*>(plant);
            if (!superBomb) {
                // Clear grid reference
                if (plant->getRow() >= 0 && plant->getRow() < ROWS &&
                    plant->getCol() >= 0 && plant->getCol() < COLS) {
                    m_grid[plant->getRow()][plant->getCol()] = nullptr;
                }
                plant->markForDeletion();
            }
        }
    }
    
    // Mark all skeletons for deletion
    for (Skeleton *skeleton : m_skeletons) {
        if (skeleton && !skeleton->isMarkedForDeletion()) {
            skeleton->markForDeletion();
        }
    }
    
    qDebug() << "SUPER NUCLEAR EXPLOSION! ALL entities destroyed!";
}

void GameScene::cycleGameSpeed()
{
    // Cycle through 1x, 3x, 5x speeds
    if (m_gameSpeedMultiplier == 1.0f) {
        m_gameSpeedMultiplier = 3.0f;
    } else if (m_gameSpeedMultiplier == 3.0f) {
        m_gameSpeedMultiplier = 5.0f;
    } else {
        m_gameSpeedMultiplier = 1.0f;
    }
    
    qDebug() << "Game speed changed to" << m_gameSpeedMultiplier << "x";
}

void GameScene::showOptionsMenu()
{
    if (!m_isRunning) return;
    
    // Pause the game
    if (!m_isPaused) {
        pauseGame();
    }
    
    // Update options menu with current settings
    m_optionsMenu->setAutoCollectEnabled(m_autoCollectSun);
    m_optionsMenu->setGameSpeed(m_gameSpeedMultiplier);
    
    // Center the options menu
    int x = (width() - m_optionsMenu->width()) / 2;
    int y = (height() - m_optionsMenu->height()) / 2;
    m_optionsMenu->move(x, y);
    m_optionsMenu->show();
    m_optionsMenu->raise();
}

void GameScene::hideOptionsMenu()
{
    m_optionsMenu->hide();
}

void GameScene::onOptionsMenuClosed()
{
    hideOptionsMenu();
    resumeGame();
}

void GameScene::onAutoCollectChanged(bool enabled)
{
    m_autoCollectSun = enabled;
    qDebug() << "自動收集陽光" << (enabled ? "Enabled" : "Disabled");
}

void GameScene::onGameSpeedChanged(float speed)
{
    m_gameSpeedMultiplier = speed;
    qDebug() << "Game speed changed to" << speed << "x";
}
