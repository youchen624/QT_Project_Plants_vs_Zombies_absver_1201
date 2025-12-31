#ifndef GAMECORE_H
#define GAMECORE_H

// setting
// #### #### || #### #### || #### ####

#define TICK_PER_MS 50
#define GRID_ROWS 5
#define GRID_COLS 9
#define GRID_START_X 250
#define GRID_START_Y 80
#define GRID_SIZE 100

// Collision detection constants
#define BULLET_COLLISION_THRESHOLD_FRONT 40
#define BULLET_COLLISION_THRESHOLD_BACK -20

// #### #### || #### #### || #### ####

#include "Global.h"

// Helper functions for grid calculations (after Qt headers are included)
inline qreal getGridX(int col, int offsetX = 25) {
    return GRID_START_X + col * GRID_SIZE + offsetX;
}

inline qreal getGridY(int row, int offsetY = 25) {
    return GRID_START_Y + row * GRID_SIZE + offsetY;
}

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
#include <QLabel>

#include "AnimationUnit.h"

namespace PVZ {
    namespace Plant { class Base; }
    namespace Zombie { class Base; }
    namespace Core { class Bullet; class Sun; }
}

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
};


class GameCore : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameCore();
    ~GameCore();

    void start() {
        state = GameState::Progressing;
        if (!m_timer.isActive()) {
            m_timer.start(TICK_PER_MS);
        }
    }

    void pause() {
        state = GameState::Pause;
        m_timer.stop();
    }

    void plantAt(QString type, int row, int col);
    void spawnZombie(QString type, int row);
    void spawnSun(qreal x, qreal y, bool fromSky = true);
    void collectSun(Sun* sun);
    
    int getSunCount() const { return sun_count; }
    void addSun(int amount) { sun_count += amount; updateSunDisplay(); }
    void removeSun(int amount) { sun_count -= amount; updateSunDisplay(); }

    QGraphicsView   *m_view;
    QGraphicsScene  *m_scene;

    QTimer m_timer;
    GameState state;

protected:
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void gameTick();

signals:
    void coreTick();

private:
    void updateSunDisplay();
    void checkCollisions();
    void cleanupDeadObjects();
    void spawnZombieWave();

    int sun_count;
    int tick_count;
    int zombie_spawn_ticks;
    
    QLabel* sun_label;
    
    // Game objects
    Plant::Base* plants[GRID_ROWS][GRID_COLS];
    QList<Zombie::Base*> zombies;
    QList<Bullet*> bullets;
    QList<Sun*> suns;
};

}; };

#endif // GAMECORE_H
