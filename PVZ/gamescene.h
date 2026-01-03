#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QWidget>
#include <QTimer>
#include <QList>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QHBoxLayout>
#include "plants/plant.h"
#include "zombies/zombie.h"
#include "projectile.h"
#include "plantcard.h"
#include "sun.h"
#include "pausemenu.h"
#include "optionsmenu.h"

class GameScene : public QWidget
{
    Q_OBJECT
public:
    explicit GameScene(QWidget *parent = nullptr);
    ~GameScene();

    void startGame(int levelId = 1);
    void pauseGame();
    void resumeGame();
    void resetGame();
    void setSelectedPlants(const QVector<PlantCard::PlantType> &plants);
    void showOptionsMenu();

    int getSunAmount() const { return m_sunAmount; }
    void addSun(int amount);
    bool spendSun(int amount);

    enum PlantType {
        PLANT_NONE = 0,
        PLANT_SUNFLOWER,
        PLANT_SHOOTER,
        PLANT_WALL,
        PLANT_ICESHOOTER,
        PLANT_REPEATER,
        PLANT_NUCLEARBOMB,
        PLANT_SKELETONTREE,
        PLANT_GROUNDSPIKE,
        PLANT_CHILIPEPPER,
        PLANT_MINE,
        PLANT_TWINSUNFLOWER,
        PLANT_CATGRASS,
        PLANT_SUPERGATLINGPEA,
        PLANT_SUPERNUCLEARBOMB,
        PLANT_SHOVEL
    };

    void selectPlantType(PlantType type);

signals:
    void sunAmountChanged(int amount);
    void gameWon();
    void gameLost();
    void exitToMainMenu();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void gameLoop();
    void spawnZombie();
    void onSunProduced(int amount);
    void spawnSunFromSky();
    void onCardClicked(PlantCard::PlantType type);
    void onPauseMenuContinue();
    void onPauseMenuShowOptions();
    void onPauseMenuExit();
    void startZombieSpawning();
    void onNuclearExplosion();
    void onSkeletonSummon(int treeRow);
    void onChiliBurn(int row);
    void onMineExplode(int row, int col);
    void onSuperNuclearExplosion();
    void onOptionsMenuClosed();
    void onAutoCollectChanged(bool enabled);
    void onGameSpeedChanged(float speed);

private:
    void updateEntities(float deltaTime);
    void checkCollisions();
    void checkWinLoseConditions();
    void drawGrid(QPainter *painter);
    void drawHUD(QPainter *painter);
    void shootProjectiles();
    void setupCards();
    void updateCards(float deltaTime);
    void showPauseMenu();
    void hidePauseMenu();
    void hideOptionsMenu();
    void cycleGameSpeed(); // Cycle through 1x, 3x, 5x speeds
    void submitScoreToLeaderboard(bool isWin);
    
    bool canPlacePlant(int row, int col) const;
    bool canUpgradePlant(int row, int col, PlantType upgradeType) const;
    void placePlant(int row, int col);
    QPoint gridToPixel(int row, int col) const;
    void pixelToGrid(const QPoint &pixel, int &row, int &col) const;

    // Game state
    bool m_isRunning;
    bool m_isPaused;
    bool m_isAdvancedPause; // Alt key pause - allows planting but freezes zombies
    int m_sunAmount;
    PlantType m_selectedPlantType;
    int m_currentWave;
    int m_zombiesSpawned;
    int m_maxZombiesPerWave;
    int m_currentLevel;
    int m_totalWaves;
    bool m_isFlagWave;
    bool m_isEndlessMode;
    float m_gameSpeedMultiplier; // Game speed control: 1.0x, 3.0x, 5.0x
    bool m_autoCollectSun; // Auto-collect sun setting
    
    // Game statistics for leaderboard
    int m_zombiesKilled;
    int m_plantsPlaced;
    int m_totalSunCollected;
    qint64 m_gameStartTime;

    // Timers
    QTimer *m_gameTimer;
    QTimer *m_spawnTimer;
    QTimer *m_sunTimer;
    QTimer *m_initialDelayTimer;
    qint64 m_lastUpdateTime;
    float m_shootTimer;

    // Grid system
public:
    static const int ROWS = 5;
    static const int COLS = 9;
private:
    static const int CELL_SIZE = 80;
    static const int GRID_OFFSET_X = 250;
    static const int GRID_OFFSET_Y = 180; // Increased from 150 to account for taller HUD

    // Entities
    QList<Plant*> m_plants;
    QList<Zombie*> m_zombies;
    QList<Projectile*> m_projectiles;
    QList<Sun*> m_suns;
    QList<class Skeleton*> m_skeletons; // Friendly skeletons that attack zombies
    Plant* m_grid[ROWS][COLS]; // Track occupied cells
    
    // UI Elements
    QList<PlantCard*> m_plantCards;
    PlantCard *m_selectedCard;
    PauseMenu *m_pauseMenu;
    OptionsMenu *m_optionsMenu;
    QVector<PlantCard::PlantType> m_selectedPlantTypes; // Plants chosen in plant selection screen
};

#endif // GAMESCENE_H
