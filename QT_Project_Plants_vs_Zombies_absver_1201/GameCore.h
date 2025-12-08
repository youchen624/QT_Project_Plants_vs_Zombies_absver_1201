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
#include "BasePlant.h"

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
    // 遊戲模式
    enum class GameState {
        Pause = 0,
        Progressing,
        GameOver_lose,
        GameOver_win
    };

    typedef struct Damage {
        quint64 value;
        enum class Type {
            Normal = 0,
            Explode,    // 爆炸
            Bite,       // 啃咬
            Burn,       // 燃燒
            Drown,      // 溺水
            Force       // 原力
        } type;
    } Damage;

    typedef struct Protection {
        enum class Type {
            Normal,
            Plastic,
            Metal
        } type;

        quint64 value;
    } Protection;

    typedef struct Weapon {
        enum class Type {
            Normal,
            Metal
        };
        virtual void attack(GameCore*) {};

        quint64 boosted;
    } Weapon;

    class GameCore : public QObject
    {
    Q_OBJECT
    signals:
        void tick(GameState);
    public:
        GameCore(QObject *parent = nullptr, bool debug = false);
        void start();
        void stop();
        void gameover();


        // find the first plant posision (for zombie in normal)
        int firstPlant_x(int y);

    private slots:
        void clock();

    private:
        // bool pause;
        GameState gameState;
        bool _debug;
        QTimer timer;
        QDateTime dateTime;

        QGraphicsScene scene;
        QGraphicsView* view;

        // 0 1 2 // more less more close the house
        QVector<QVector<QList<
            Plant::BasePlant*>>> plantsGrid;

        QList<PVZ::Object::ItemRenderable*>itemRender;
        // QList<> // items
    };
};
};

#endif // GAMECORE_H
