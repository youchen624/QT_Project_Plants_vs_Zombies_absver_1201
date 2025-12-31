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

private slots:
    void onPlantPlaced(int row, int col);
    void onZombieSpawned(Zombie* zombie);
    void onTickUpdate(int tickCount);

private:
    void drawGrid();
    void drawPlants();
    void drawZombies();
    QGraphicsPixmapItem* createEntityItem(const QPixmap& image, int x, int y);

    Game* game;
    int cellWidth;
    int cellHeight;
    
    // Map to track visual items
    QMap<Plant*, QGraphicsPixmapItem*> plantItems;
    QMap<Zombie*, QGraphicsPixmapItem*> zombieItems;
};

#endif // GAMESCENE_H
