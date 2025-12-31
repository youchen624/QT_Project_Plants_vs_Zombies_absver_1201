#ifndef SUN_H
#define SUN_H

#include "core/ObjectTickable.h"
#include <QGraphicsEllipseItem>

namespace PVZ {
namespace Core {

class Sun : public ObjectTickable
{
    Q_OBJECT
public:
    Sun(GameCore* core, qreal x, qreal y, bool fromSky = false);
    ~Sun();

    void tick(GameState state) override;
    
    bool isCollectable() const { return collectable; }
    void collect();
    int getValue() const { return value; }

protected:
    qreal x, y;
    qreal targetY;
    qreal speed;
    int value;
    int lifetime;
    bool collectable;
    bool collected;
    bool fromSky;
    QGraphicsEllipseItem* graphic;
};

}; };

#endif // SUN_H
