#include "Game.h"
#include "Sunflower.h"
#include "Peashooter.h"
#include "NormalZombie.h"

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
    PlantCell* cell = getCell(row, col);
    if (!cell || cell->isOccupied()) {
        return false;
    }
    
    // Check if we have enough sun points
    if (sunPoints < plant->getCost()) {
        return false;
    }
    
    // Place the plant
    if (cell->placePlant(plant)) {
        spendSunPoints(plant->getCost());
        
        // Connect signals based on plant type
        Sunflower* sunflower = qobject_cast<Sunflower*>(plant);
        if (sunflower) {
            connect(sunflower, &Sunflower::sunProduced, this, &Game::onSunProduced);
        }
        
        return true;
    }
    
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
        
        // Check each column in the zombie's row
        for (int col = 0; col < gridCols; ++col) {
            PlantCell* cell = grid[zombieRow][col];
            if (cell && cell->isOccupied()) {
                Plant* plant = cell->getPlant();
                if (plant && plant->isAlive()) {
                    // Simple collision: zombie reaches plant's column
                    int plantX = col * 100;  // Assuming each cell is 100 pixels wide
                    
                    if (zombieX <= plantX + 100 && zombieX >= plantX) {
                        // Zombie is at the plant's position
                        NormalZombie* normalZombie = qobject_cast<NormalZombie*>(zombie);
                        if (normalZombie) {
                            normalZombie->setAttacking(true);
                            // Attack the plant
                            plant->takeDamage(zombie->getAttackDamage() / 30);  // Divide by tick rate
                        }
                        break;  // Only attack one plant at a time
                    } else if (zombieX < plantX) {
                        // Passed this plant, stop attacking
                        NormalZombie* normalZombie = qobject_cast<NormalZombie*>(zombie);
                        if (normalZombie) {
                            normalZombie->setAttacking(false);
                        }
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
}
