#ifndef GAMECORE_H
#define GAMECORE_H

// setting
// #### #### || #### #### || #### ####

#define TICK_PER_MS 50

// #### #### || #### #### || #### ####

#include "Global.h"

#include <QMainWindow>
#include <QObject>
#include <QList>
#include <QVector>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QPixmapCache>
#include <QBrush>
#include <QPen>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QDateTime>
#include <QTimer>
#include <QDebug>

#include "AnimationUnit.h"

#include "plant/Sunflower.h"

namespace PVZ {
namespace Core {
class GameCore;

class Ground {
public:
    Ground(GameCore* core);
    ~Ground();

protected:
    GameCore* core;
    QGraphicsPixmapItem graphic_main;

    static QPixmap pix;

private:
    // bool plantable;
};


class GameCore : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameCore();

    void start() {
        state = GameState::Progressing;
        if (!m_timer.isActive()) {
            m_timer.start(TICK_PER_MS);
        }

        // m_view->show();

    }

    void pause() {
        state = GameState::Pause;
        m_timer.stop();
    }

    Plant::Sunflower sflower;

    void plant(QString type, int row, int column){};


    // void addSceneItem(AnimationUnit aniUnit);
    //

// private:
    QGraphicsView   *m_view;
    QGraphicsScene  *m_scene;

    QTimer m_timer;
    GameState state;

private slots:
signals:
    void coreTick();
};

}; };

#endif // GAMECORE_H
