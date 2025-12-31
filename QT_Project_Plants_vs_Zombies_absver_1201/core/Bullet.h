#ifndef BULLET_H
#define BULLET_H

#include "core/ObjectTickable.h"
#include "core/Global.h"
#include <QGraphicsEllipseItem>

namespace PVZ {
namespace Core {

class Bullet : public ObjectTickable
{
    Q_OBJECT
public:
    Bullet(GameCore* core, qreal x, qreal y, int row);
    ~Bullet();

    void tick(GameState state) override;
    
    qreal getX() const { return x; }
    qreal getY() const { return y; }
    int getRow() const { return row; }
    bool isActive() const { return active; }
    void deactivate() { active = false; }

    Damage getDamage() const { return damage; }

protected:
    qreal x, y;
    int row;
    qreal speed;
    bool active;
    Damage damage;
    QGraphicsEllipseItem* graphic;
};

}; };

#endif // BULLET_H
