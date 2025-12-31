#include "Game.h"
#include "Sunflower.h"
#include "Peashooter.h"
#include "NormalZombie.h"
#include <QDebug>

// Game constants
static const int CELL_WIDTH = 100;  // Width of each grid cell in pixels

Game::Game(QObject *parent)
    : QObject(parent)
    , gameTimer(new QTimer(this))
    , running(false)
    , paused(false)
    , tickCount(0)
    , tickRate(30)              // 30 ticks per second
    , sunPoints(150)            // Start with 150 sun points
    , initialSunPoints(150)
    , gridRows(5)               // 5 rows
    , gridCols(9)               // 9 columns
{
    // Initialize game timer
    gameTimer->setInterval(1000 / tickRate);  // Convert to milliseconds
    connect(gameTimer, &QTimer::timeout, this, &Game::gameTick);
    
    // Initialize the grid
    initializeGrid();
}

Game::~Game()
{
    stop();
    
    // Clean up grid
    for (int row = 0; row < gridRows; ++row) {
        for (int col = 0; col < gridCols; ++col) {
            delete grid[row][col];
        }
    }
    
    // Clean up zombies
    qDeleteAll(zombies);
    zombies.clear();
}

void Game::start()
{
    if (!running) {
        running = true;
        paused = false;
        tickCount = 0;
        sunPoints = initialSunPoints;
        
        gameTimer->start();
        emit gameStarted();
        emit sunPointsChanged(sunPoints);
    }
}

void Game::stop()
{
    if (running) {
        running = false;
        paused = false;
        gameTimer->stop();
        emit gameStopped();
    }
}

void Game::pause()
{
    if (running && !paused) {
        paused = true;
        gameTimer->stop();
        emit gamePaused();
    }
}

void Game::resume()
{
    if (running && paused) {
        paused = false;
        gameTimer->start();
        emit gameResumed();
    }
}

void Game::initializeGrid()
{
    grid.resize(gridRows);
    for (int row = 0; row < gridRows; ++row) {
        grid[row].resize(gridCols);
        for (int col = 0; col < gridCols; ++col) {
            grid[row][col] = new PlantCell(row, col, this);
        }
    }
}

PlantCell* Game::getCell(int row, int col)
{
    if (row >= 0 && row < gridRows && col >= 0 && col < gridCols) {
        return grid[row][col];
    }
    return nullptr;
}

bool Game::placePlant(Plant* plant, int row, int col)
{
    qDebug() << "placePlant called for row" << row << "col" << col;
    
    PlantCell* cell = getCell(row, col);
    if (!cell) {
        qDebug() << "Cell is null";
        return false;
    }
    
    if (cell->isOccupied()) {
        qDebug() << "Cell is already occupied";
        return false;
    }
    
    // Check if we have enough sun points
    qDebug() << "Sun points:" << sunPoints << "Plant cost:" << plant->getCost();
    if (sunPoints < plant->getCost()) {
        qDebug() << "Not enough sun points";
        return false;
    }
    
    // Place the plant
    if (cell->placePlant(plant)) {
        spendSunPoints(plant->getCost());
        qDebug() << "Plant placed successfully. Remaining sun points:" << sunPoints;
        
        // Connect signals based on plant type
        Sunflower* sunflower = qobject_cast<Sunflower*>(plant);
        if (sunflower) {
            connect(sunflower, &Sunflower::sunProduced, this, &Game::onSunProduced);
        }
        
        Peashooter* peashooter = qobject_cast<Peashooter*>(plant);
        if (peashooter) {
            connect(peashooter, &Peashooter::peaShot, this, &Game::onPeaShot);
        }
        
        return true;
    }
    
    qDebug() << "Cell placePlant failed";
    return false;
}

void Game::spawnZombie(Zombie* zombie)
{
    if (zombie) {
        zombies.append(zombie);
        zombie->setParent(this);
        
        // Connect zombie signals
        connect(zombie, &Zombie::died, this, &Game::onZombieDied);
        connect(zombie, &Zombie::reachedEnd, this, &Game::onZombieReachedEnd);
        
        // Connect attack signal for NormalZombie
        NormalZombie* normalZombie = qobject_cast<NormalZombie*>(zombie);
        if (normalZombie) {
            connect(normalZombie, &NormalZombie::attackingPlant, this, &Game::onZombieAttackingPlant);
        }
        
        emit zombieSpawned(zombie);
    }
}

void Game::addSunPoints(int amount)
{
    sunPoints += amount;
    emit sunPointsChanged(sunPoints);
}

bool Game::spendSunPoints(int amount)
{
    if (sunPoints >= amount) {
        sunPoints -= amount;
        emit sunPointsChanged(sunPoints);
        return true;
    }
    return false;
}

void Game::gameTick()
{
    if (!running || paused) {
        return;
    }
    
    tickCount++;
    
    // Update all game entities
    updatePlants();
    updateZombies();
    checkCollisions();
    cleanupDeadEntities();
    
    emit tickUpdate(tickCount);
}

void Game::updatePlants()
{
    for (int row = 0; row < gridRows; ++row) {
        for (int col = 0; col < gridCols; ++col) {
            grid[row][col]->update(tickCount);
        }
    }
}

void Game::updateZombies()
{
    for (Zombie* zombie : zombies) {
        if (zombie && zombie->isAlive()) {
            zombie->update(tickCount);
        }
    }
}

void Game::checkCollisions()
{
    // Check if zombies should attack plants
    for (Zombie* zombie : zombies) {
        if (!zombie || !zombie->isAlive()) {
            continue;
        }
        
        int zombieRow = zombie->getPosition().y();
        int zombieX = zombie->getPosition().x();
        
        // Assume zombie is not attacking until we find a plant
        NormalZombie* normalZombie = qobject_cast<NormalZombie*>(zombie);
        if (normalZombie) {
            normalZombie->setAttacking(false);
        }
        
        // Check each column in the zombie's row
        for (int col = 0; col < gridCols; ++col) {
            PlantCell* cell = grid[zombieRow][col];
            if (cell && cell->isOccupied()) {
                Plant* plant = cell->getPlant();
                if (plant && plant->isAlive()) {
                    // Simple collision: zombie reaches plant's column
                    int plantX = col * CELL_WIDTH;
                    
                    if (zombieX <= plantX + CELL_WIDTH && zombieX >= plantX) {
                        // Zombie is at the plant's position - attack it
                        if (normalZombie) {
                            normalZombie->setAttacking(true);
                            // Zombie attacks on its own cooldown, not every tick
                            // Damage is handled by the zombie's attack() method
                        }
                        break;  // Only attack one plant at a time
                    }
                }
            }
        }
    }
}

void Game::cleanupDeadEntities()
{
    // Remove dead zombies
    for (int i = zombies.size() - 1; i >= 0; --i) {
        if (zombies[i] && !zombies[i]->isAlive()) {
            Zombie* zombie = zombies.takeAt(i);
            zombie->deleteLater();
        }
    }
}

void Game::onZombieDied()
{
    Zombie* zombie = qobject_cast<Zombie*>(sender());
    if (zombie) {
        emit zombieDied(zombie);
    }
}

void Game::onZombieReachedEnd()
{
    // Game over - zombie reached the end
    stop();
    emit gameOver(false);  // Player lost
}

void Game::onSunProduced(int amount)
{
    addSunPoints(amount);
    qDebug() << "Sun produced! Amount:" << amount << "New total:" << sunPoints;
    
    // Find which sunflower produced the sun for visual feedback
    Sunflower* sender = qobject_cast<Sunflower*>(QObject::sender());
    if (sender) {
        QPoint pos = sender->getPosition();
        emit sunProducedVisual(pos.y(), pos.x(), amount);
        qDebug() << "Sun produced at position:" << pos;
    }
}

void Game::onPeaShot(int row, int damage)
{
    qDebug() << "Pea shot in row" << row << "with damage" << damage;
    
    // Find which peashooter shot for visual feedback
    Peashooter* sender = qobject_cast<Peashooter*>(QObject::sender());
    if (sender) {
        QPoint pos = sender->getPosition();
        emit peaShotVisual(pos.y(), pos.x());
    }
    
    // Find the first zombie in this row and damage it
    for (Zombie* zombie : zombies) {
        if (zombie && zombie->isAlive() && zombie->getPosition().y() == row) {
            zombie->takeDamage(damage);
            qDebug() << "Zombie hit! Remaining health:" << zombie->getHealth();
            break; // Only hit one zombie per shot
        }
    }
}

void Game::onZombieAttackingPlant(int row, int damage)
{
    qDebug() << "Zombie attacking plant in row" << row << "with damage" << damage;
    
    // Find which zombie is attacking to get its position
    NormalZombie* attackingZombie = qobject_cast<NormalZombie*>(QObject::sender());
    if (!attackingZombie) {
        return;
    }
    
    int zombieX = attackingZombie->getPosition().x();
    
    // Find the plant that the zombie is attacking
    for (int col = 0; col < gridCols; ++col) {
        PlantCell* cell = grid[row][col];
        if (cell && cell->isOccupied()) {
            Plant* plant = cell->getPlant();
            if (plant && plant->isAlive()) {
                int plantX = col * CELL_WIDTH;
                
                // Check if zombie is at this plant's position
                if (zombieX <= plantX + CELL_WIDTH && zombieX >= plantX) {
                    plant->takeDamage(damage);
                    qDebug() << "Plant hit! Remaining health:" << plant->getHealth();
                    return;
                }
            }
        }
    }
}
