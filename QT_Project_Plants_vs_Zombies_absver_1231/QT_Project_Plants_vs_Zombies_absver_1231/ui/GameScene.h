#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMap>
#include "Game.h"

// Visual rendering for the game using QGraphicsScene
class GameScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GameScene(Game* game, QObject *parent = nullptr);
    ~GameScene();

    void setupScene();
    void updateScene();
    
    // Plant selection for placement
    void setPlantToPlace(const QString& plantType);
    void cancelPlantPlacement();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private slots:
    void onPlantPlaced(int row, int col);
    void onZombieSpawned(Zombie* zombie);
    void onTickUpdate(int tickCount);
    void onSunProducedVisual(int row, int col, int amount);
    void onPeaShotVisual(int row, int col);

private:
    void drawGrid();
    void drawPlants();
    void drawZombies();
    QGraphicsPixmapItem* createEntityItem(const QPixmap& image, int x, int y);
    
    // Convert pixel position to grid coordinates
    int pixelToRow(int y) const;
    int pixelToCol(int x) const;

    Game* game;
    int cellWidth;
    int cellHeight;
    
    // Map to track visual items
    QMap<Plant*, QGraphicsPixmapItem*> plantItems;
    QMap<Zombie*, QGraphicsPixmapItem*> zombieItems;
    
    // Plant placement state
    QString selectedPlantType;
    bool isPlacingPlant;
};

#endif // GAMESCENE_H
