#include "Sun.h"
#include "GameCore.h"
#include <QBrush>

using namespace PVZ::Core;

Sun::Sun(GameCore* core, qreal x, qreal y, bool fromSky) :
    ObjectTickable(core), x(x), y(y), targetY(fromSky ? y + 200 : y),
    speed(2.0), value(25), lifetime(500), collectable(false), 
    collected(false), fromSky(fromSky)
{
    // Create visual representation
    graphic = new QGraphicsEllipseItem(0, 0, 40, 40);
    graphic->setBrush(QBrush(QColor(255, 255, 0)));
    graphic->setPos(x, y);
    core->m_scene->addItem(graphic);
}

Sun::~Sun() {
    if (graphic && core && core->m_scene) {
        core->m_scene->removeItem(graphic);
        delete graphic;
    }
}

void Sun::tick(GameState state) {
    if (state != GameState::Progressing || collected) return;
    
    // Fall from sky or stay in place
    if (fromSky && y < targetY) {
        y += speed;
        graphic->setPos(x, y);
    } else {
        collectable = true;
    }
    
    // Decrease lifetime
    if (collectable) {
        lifetime--;
        if (lifetime <= 0) {
            collected = true;
        }
    }
}

void Sun::collect() {
    collected = true;
}
