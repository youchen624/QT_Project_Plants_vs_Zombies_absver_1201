#include "GameScene.h"
#include "Sunflower.h"
#include "Peashooter.h"
#include "Projectile.h"
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QPen>
#include <QDebug>
#include <QTimer>

GameScene::GameScene(Game* game, QObject *parent)
    : QGraphicsScene(parent)
    , game(game)
    , cellWidth(100)
    , cellHeight(100)
    , isPlacingPlant(false)
{
    setupScene();
    
    // Connect to game signals
    connect(game, &Game::tickUpdate, this, &GameScene::onTickUpdate);
    connect(game, &Game::zombieSpawned, this, &GameScene::onZombieSpawned);
    connect(game, &Game::sunProducedVisual, this, &GameScene::onSunProducedVisual);
    connect(game, &Game::peaShotVisual, this, &GameScene::onPeaShotVisual);
    
    // Connect to all plant cells to get plant placement notifications
    for (int row = 0; row < game->getGridRows(); ++row) {
        for (int col = 0; col < game->getGridCols(); ++col) {
            PlantCell* cell = game->getCell(row, col);
            if (cell) {
                connect(cell, &PlantCell::plantPlaced, this, [this](Plant*) {
                    qDebug() << "Plant placed signal received, updating scene";
                    updateScene();
                });
            }
        }
    }
    
    qDebug() << "GameScene initialized";
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
    qDebug() << "drawPlants called";
    
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
    int plantCount = 0;
    for (int row = 0; row < game->getGridRows(); ++row) {
        for (int col = 0; col < game->getGridCols(); ++col) {
            PlantCell* cell = game->getCell(row, col);
            if (cell && cell->isOccupied()) {
                Plant* plant = cell->getPlant();
                if (plant && plant->isAlive()) {
                    plantCount++;
                    // Check if we already have an item for this plant
                    if (!plantItems.contains(plant)) {
                        int x = col * cellWidth + cellWidth / 2 - 32;
                        int y = row * cellHeight + cellHeight / 2 - 32;
                        qDebug() << "Creating visual item for plant at grid (" << row << "," << col << ") pixel position (" << x << "," << y << ")";
                        QGraphicsPixmapItem* item = createEntityItem(plant->getImage(), x, y);
                        plantItems[plant] = item;
                    }
                }
            }
        }
    }
    qDebug() << "Total plants in scene:" << plantCount << "Visual items:" << plantItems.size();
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
    if (image.isNull()) {
        qWarning() << "Attempting to create item with null pixmap at position" << x << "," << y;
        // Create a colored rectangle as a fallback
        QPixmap fallback(64, 64);
        fallback.fill(Qt::red);
        QGraphicsPixmapItem* item = addPixmap(fallback);
        item->setPos(x, y);
        return item;
    }
    
    QGraphicsPixmapItem* item = addPixmap(image);
    item->setPos(x, y);
    qDebug() << "Created entity item at" << x << "," << y << "with image size" << image.size();
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

void GameScene::setPlantToPlace(const QString& plantType)
{
    selectedPlantType = plantType;
    isPlacingPlant = true;
    qDebug() << "Plant placement mode activated for:" << plantType;
}

void GameScene::cancelPlantPlacement()
{
    selectedPlantType.clear();
    isPlacingPlant = false;
    qDebug() << "Plant placement mode cancelled";
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (isPlacingPlant && !selectedPlantType.isEmpty()) {
        // Convert mouse position to grid coordinates
        int col = pixelToCol(event->scenePos().x());
        int row = pixelToRow(event->scenePos().y());
        
        qDebug() << "Clicked at grid position:" << row << "," << col;
        
        // Validate grid bounds
        if (row >= 0 && row < game->getGridRows() && col >= 0 && col < game->getGridCols()) {
            // Create the appropriate plant
            Plant* plant = nullptr;
            if (selectedPlantType == "Sunflower") {
                plant = new Sunflower();
            } else if (selectedPlantType == "Peashooter") {
                plant = new Peashooter();
            }
            
            if (plant) {
                if (game->placePlant(plant, row, col)) {
                    qDebug() << "Successfully placed" << selectedPlantType << "at (" << row << "," << col << ")";
                    // Don't cancel placement mode - allow multiple plants
                } else {
                    qDebug() << "Failed to place" << selectedPlantType;
                    delete plant;
                }
            }
        } else {
            qDebug() << "Click outside grid bounds";
        }
    }
    
    QGraphicsScene::mousePressEvent(event);
}

int GameScene::pixelToRow(int y) const
{
    return y / cellHeight;
}

int GameScene::pixelToCol(int x) const
{
    return x / cellWidth;
}

void GameScene::onSunProducedVisual(int row, int col, int amount)
{
    qDebug() << "Visual sun produced at (" << row << "," << col << ") amount:" << amount;
    
    // Create a temporary visual indicator for sun production
    int x = col * cellWidth + cellWidth / 2 - 15;
    int y = row * cellHeight + cellHeight / 2 - 15;
    
    // Create a yellow circle to represent sun
    QGraphicsEllipseItem* sunItem = addEllipse(x, y, 30, 30, 
                                                QPen(QColor(255, 165, 0), 2), 
                                                QBrush(QColor(255, 220, 0)));
    
    // Add text showing amount
    QGraphicsTextItem* textItem = addText(QString::number(amount));
    textItem->setPos(x + 8, y + 5);
    textItem->setDefaultTextColor(Qt::black);
    
    // Remove after 1 second (using QTimer)
    QTimer::singleShot(1000, this, [this, sunItem, textItem]() {
        removeItem(sunItem);
        removeItem(textItem);
        delete sunItem;
        delete textItem;
    });
}

void GameScene::onPeaShotVisual(int row, int col)
{
    qDebug() << "Visual pea shot from (" << row << "," << col << ")";
    
    // Create animated projectile
    int startX = col * cellWidth + cellWidth - 10;
    int startY = row * cellHeight + cellHeight / 2 - 5;
    
    Projectile* projectile = new Projectile(startX, startY, row, 20, this, this);
    projectiles.append(projectile);
    
    // Connect projectile destroyed signal for cleanup
    connect(projectile, &Projectile::destroyed, this, [this, projectile]() {
        projectiles.removeOne(projectile);
    });
    
    // Start projectile animation
    projectile->start();
}
