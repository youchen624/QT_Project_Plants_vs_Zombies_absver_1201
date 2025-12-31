#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include "Game.h"
#include "Sunflower.h"
#include "Peashooter.h"
#include "NormalZombie.h"

// Simple test class to verify the game logic
class GameTester : public QObject
{
    Q_OBJECT

public:
    GameTester(Game* game) : game(game) {
        connect(game, &Game::gameStarted, this, &GameTester::onGameStarted);
        connect(game, &Game::sunPointsChanged, this, &GameTester::onSunPointsChanged);
        connect(game, &Game::tickUpdate, this, &GameTester::onTickUpdate);
        connect(game, &Game::zombieSpawned, this, &GameTester::onZombieSpawned);
        connect(game, &Game::zombieDied, this, &GameTester::onZombieDied);
    }

public slots:
    void runTests() {
        qDebug() << "=== Starting Game Tests ===";
        
        // Test 1: Initial state
        qDebug() << "Test 1: Initial game state";
        qDebug() << "  Grid size:" << game->getGridRows() << "x" << game->getGridCols();
        qDebug() << "  Initial sun points:" << game->getSunPoints();
        qDebug() << "  Is running:" << game->isRunning();
        
        // Test 2: Start game
        qDebug() << "\nTest 2: Starting game";
        game->start();
        
        // Test 3: Place plants (after a short delay to let game start)
        QTimer::singleShot(100, this, &GameTester::testPlacePlants);
        
        // Test 4: Spawn zombie (after plants are placed)
        QTimer::singleShot(200, this, &GameTester::testSpawnZombie);
        
        // Test 5: Check game progress
        QTimer::singleShot(500, this, &GameTester::checkProgress);
        
        // Stop after 2 seconds
        QTimer::singleShot(2000, this, &GameTester::stopTests);
    }

private slots:
    void onGameStarted() {
        qDebug() << "  Game started successfully";
    }

    void onSunPointsChanged(int amount) {
        qDebug() << "  Sun points changed to:" << amount;
    }

    void onTickUpdate(int tickCount) {
        if (tickCount % 30 == 0) {  // Log every 30 ticks (1 second)
            qDebug() << "  Tick:" << tickCount;
        }
    }

    void onZombieSpawned(Zombie* zombie) {
        qDebug() << "  Zombie spawned:" << zombie->getName() 
                 << "at row" << zombie->getPosition().y();
    }

    void onZombieDied(Zombie* zombie) {
        qDebug() << "  Zombie died:" << zombie->getName();
    }

    void testPlacePlants() {
        qDebug() << "\nTest 3: Placing plants";
        
        // Place Sunflower
        Sunflower* sunflower = new Sunflower();
        if (game->placePlant(sunflower, 2, 1)) {
            qDebug() << "  Placed Sunflower at (2, 1) - Cost:" << sunflower->getCost();
        } else {
            qDebug() << "  Failed to place Sunflower";
            delete sunflower;
        }
        
        // Place Peashooter
        Peashooter* peashooter = new Peashooter();
        if (game->placePlant(peashooter, 2, 3)) {
            qDebug() << "  Placed Peashooter at (2, 3) - Cost:" << peashooter->getCost();
        } else {
            qDebug() << "  Failed to place Peashooter";
            delete peashooter;
        }
    }

    void testSpawnZombie() {
        qDebug() << "\nTest 4: Spawning zombie";
        NormalZombie* zombie = new NormalZombie(2);
        game->spawnZombie(zombie);
        qDebug() << "  Spawned Normal Zombie - Health:" << zombie->getHealth();
    }

    void checkProgress() {
        qDebug() << "\nTest 5: Checking game progress";
        qDebug() << "  Current tick:" << game->getTickCount();
        qDebug() << "  Current sun points:" << game->getSunPoints();
        qDebug() << "  Active zombies:" << game->getZombies().size();
    }

    void stopTests() {
        qDebug() << "\n=== Tests Complete ===";
        qDebug() << "Final state:";
        qDebug() << "  Total ticks:" << game->getTickCount();
        qDebug() << "  Final sun points:" << game->getSunPoints();
        qDebug() << "  Active zombies:" << game->getZombies().size();
        
        game->stop();
        QCoreApplication::quit();
    }

private:
    Game* game;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    Game game;
    GameTester tester(&game);
    
    // Start tests after event loop begins
    QTimer::singleShot(0, &tester, &GameTester::runTests);
    
    return a.exec();
}

#include "test_game.moc"
