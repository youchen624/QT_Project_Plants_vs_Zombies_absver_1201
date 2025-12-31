#include "Bullet.h"
#include "GameCore.h"
#include <QBrush>

using namespace PVZ::Core;

Bullet::Bullet(GameCore* core, qreal x, qreal y, int row) :
    ObjectTickable(core), x(x), y(y), row(row), speed(5.0), active(true)
{
    damage.value = 20;
    damage.type = Damage::Type::Normal;
    
    // Create visual representation
    graphic = new QGraphicsEllipseItem(0, 0, 20, 20);
    graphic->setBrush(QBrush(QColor(0, 255, 0)));
    graphic->setPos(x, y);
    core->m_scene->addItem(graphic);
}

Bullet::~Bullet() {
    if (graphic && core && core->m_scene) {
        core->m_scene->removeItem(graphic);
        delete graphic;
    }
}

void Bullet::tick(GameState state) {
    if (state != GameState::Progressing || !active) return;
    
    // Move bullet to the right
    x += speed;
    graphic->setPos(x, y);
    
    // Deactivate if off screen
    if (x > 1400) {
        active = false;
    }
}
