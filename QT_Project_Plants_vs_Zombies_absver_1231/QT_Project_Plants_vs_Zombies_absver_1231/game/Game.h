#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QTimer>
#include <QVector>
#include <QList>
#include "PlantCell.h"
#include "Zombie.h"

// Main game class that manages game state and tick-based logic
class Game : public QObject
{
    Q_OBJECT

public:
    explicit Game(QObject *parent = nullptr);
    ~Game();

    // Game control
    void start();
    void stop();
    void pause();
    void resume();
    bool isRunning() const { return running; }
    bool isPaused() const { return paused; }

    // Game state getters
    int getSunPoints() const { return sunPoints; }
    int getTickCount() const { return tickCount; }
    int getGridRows() const { return gridRows; }
    int getGridCols() const { return gridCols; }
    
    // Grid management
    PlantCell* getCell(int row, int col);
    bool placePlant(Plant* plant, int row, int col);
    
    // Zombie management
    void spawnZombie(Zombie* zombie);
    QList<Zombie*> getZombies() const { return zombies; }
    
    // Resource management
    void addSunPoints(int amount);
    bool spendSunPoints(int amount);

signals:
    void gameStarted();
    void gamePaused();
    void gameResumed();
    void gameStopped();
    void gameOver(bool won);
    void sunPointsChanged(int newAmount);
    void sunProducedVisual(int row, int col, int amount); // Visual sun production
    void peaShotVisual(int row, int col); // Visual pea shooting
    void tickUpdate(int tickCount);
    void zombieSpawned(Zombie* zombie);
    void zombieDied(Zombie* zombie);

private slots:
    void gameTick();               // Main game tick
    void onZombieDied();          // Handle zombie death
    void onZombieReachedEnd();    // Handle zombie reaching end
    void onSunProduced(int amount); // Handle sun production
    void onPeaShot(int row, int damage); // Handle pea shooting

private:
    void initializeGrid();         // Initialize the plant grid
    void updatePlants();           // Update all plants
    void updateZombies();          // Update all zombies
    void checkCollisions();        // Check plant-zombie collisions
    void cleanupDeadEntities();    // Remove dead plants and zombies

    QTimer* gameTimer;             // Main game timer
    bool running;                  // Game running state
    bool paused;                   // Game paused state
    int tickCount;                 // Current tick count
    int tickRate;                  // Ticks per second (default: 30)
    
    // Game resources
    int sunPoints;                 // Current sun points
    int initialSunPoints;          // Starting sun points
    
    // Grid system
    int gridRows;                  // Number of rows (default: 5)
    int gridCols;                  // Number of columns (default: 9)
    QVector<QVector<PlantCell*>> grid;  // 2D grid of plant cells
    
    // Entities
    QList<Zombie*> zombies;        // Active zombies
};

#endif // GAME_H
