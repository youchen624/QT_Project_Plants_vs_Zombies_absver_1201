#include "GameScene.h"
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>

GameScene::GameScene(Game* game, QObject *parent)
    : QGraphicsScene(parent)
    , game(game)
    , cellWidth(100)
    , cellHeight(100)
{
    setupScene();
    
    // Connect to game signals
    connect(game, &Game::tickUpdate, this, &GameScene::onTickUpdate);
    connect(game, &Game::zombieSpawned, this, &GameScene::onZombieSpawned);
}

GameScene::~GameScene()
{
}

void GameScene::setupScene()
{
    // Set scene size based on grid (9 cols x 5 rows)
    setSceneRect(0, 0, cellWidth * 9, cellHeight * 5);
    setBackgroundBrush(QBrush(QColor(139, 195, 74))); // Light green background
    
    drawGrid();
}

void GameScene::drawGrid()
{
    // Draw grid lines
    QPen gridPen(QColor(100, 150, 50), 2);
    
    // Draw vertical lines
    for (int col = 0; col <= 9; ++col) {
        int x = col * cellWidth;
        addLine(x, 0, x, cellHeight * 5, gridPen);
    }
    
    // Draw horizontal lines
    for (int row = 0; row <= 5; ++row) {
        int y = row * cellHeight;
        addLine(0, y, cellWidth * 9, y, gridPen);
    }
}

void GameScene::updateScene()
{
    drawPlants();
    drawZombies();
}

void GameScene::drawPlants()
{
    // Clear old plant items that are no longer valid
    QList<Plant*> deadPlants;
    for (auto it = plantItems.begin(); it != plantItems.end(); ++it) {
        Plant* plant = it.key();
        if (!plant || !plant->isAlive()) {
            deadPlants.append(plant);
        }
    }
    
    for (Plant* plant : deadPlants) {
        QGraphicsPixmapItem* item = plantItems.take(plant);
        if (item) {
            removeItem(item);
            delete item;
        }
    }
    
    // Draw all plants from the grid
    for (int row = 0; row < game->getGridRows(); ++row) {
        for (int col = 0; col < game->getGridCols(); ++col) {
            PlantCell* cell = game->getCell(row, col);
            if (cell && cell->isOccupied()) {
                Plant* plant = cell->getPlant();
                if (plant && plant->isAlive()) {
                    // Check if we already have an item for this plant
                    if (!plantItems.contains(plant)) {
                        int x = col * cellWidth + cellWidth / 2 - 32;
                        int y = row * cellHeight + cellHeight / 2 - 32;
                        QGraphicsPixmapItem* item = createEntityItem(plant->getImage(), x, y);
                        plantItems[plant] = item;
                    }
                }
            }
        }
    }
}

void GameScene::drawZombies()
{
    // Remove items for dead zombies
    QList<Zombie*> deadZombies;
    for (auto it = zombieItems.begin(); it != zombieItems.end(); ++it) {
        Zombie* zombie = it.key();
        if (!zombie || !zombie->isAlive()) {
            deadZombies.append(zombie);
        }
    }
    
    for (Zombie* zombie : deadZombies) {
        QGraphicsPixmapItem* item = zombieItems.take(zombie);
        if (item) {
            removeItem(item);
            delete item;
        }
    }
    
    // Update positions of existing zombies and add new ones
    QList<Zombie*> zombies = game->getZombies();
    for (Zombie* zombie : zombies) {
        if (zombie && zombie->isAlive()) {
            if (zombieItems.contains(zombie)) {
                // Update position
                QGraphicsPixmapItem* item = zombieItems[zombie];
                QPoint pos = zombie->getPosition();
                item->setPos(pos.x() - 32, pos.y() * cellHeight + cellHeight / 2 - 32);
            } else {
                // Create new item
                QPoint pos = zombie->getPosition();
                int x = pos.x() - 32;
                int y = pos.y() * cellHeight + cellHeight / 2 - 32;
                QGraphicsPixmapItem* item = createEntityItem(zombie->getImage(), x, y);
                zombieItems[zombie] = item;
            }
        }
    }
}

QGraphicsPixmapItem* GameScene::createEntityItem(const QPixmap& image, int x, int y)
{
    QGraphicsPixmapItem* item = addPixmap(image);
    item->setPos(x, y);
    return item;
}

void GameScene::onPlantPlaced(int row, int col)
{
    updateScene();
}

void GameScene::onZombieSpawned(Zombie* zombie)
{
    updateScene();
}

void GameScene::onTickUpdate(int tickCount)
{
    // Update scene every tick
    updateScene();
}
