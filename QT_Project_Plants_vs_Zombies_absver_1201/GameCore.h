#ifndef GAMECORE_H
#define GAMECORE_H

#include <QObject>
#include <QList>
#include <QVector>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QDateTime>
#include <QTimer>
#include <QDebug>

// #include "ItemRenderable.h"
// #include "ItemTickable.h"

// 50ms/tick == 20ticks/s
#define TICK_PER_MS 50

// #### #### #### ####
// #TODO file system (for adv)
// #TODO make the "Particle.class" (Object Pool, recyclable)
// #TODO make the "SoundManager.class"
// #### #### #### ####

/*
 * items
 * - gui
 * - map
 * - plant
 * - zombie
 * - misc
*/

namespace PVZ {

namespace Object {
class ItemTickable;
class ItemRenderable;
};
namespace Zombie {
class BaseZombie;
};
namespace Plant {
class BasePlant;
};

namespace Core {
    class GameCore : public QObject
    {
    public:
        GameCore(QObject *parent = nullptr, bool debug = false);
        void start();
        void stop();
        void gameover();


        int firstPlant_x(int y);

    private slots:
        void tick();

    private:
        bool pause;
        bool _debug;
        QTimer timer;
        QDateTime dateTime;

        QGraphicsScene scene;
        QGraphicsView* view;

        QVector<QVector<QList<
            Plant::BasePlant*>>> plantsGrid;

        QList<PVZ::Object::ItemRenderable*> itemRender;
        // QList<> // items
    };
};
};

#endif // GAMECORE_H
