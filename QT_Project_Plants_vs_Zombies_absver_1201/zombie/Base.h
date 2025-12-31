#ifndef ZOMBIE_BASE_H
#define ZOMBIE_BASE_H

#include "core/ObjectTickable.h"
#include "core/Global.h"
#include "core/AnimationUnit.h"

namespace PVZ {
namespace Zombie {

class Base : public Core::ObjectTickable
{
    Q_OBJECT
public:
    Base(Core::GameCore* core, int row);
    virtual ~Base();

    void tick(Core::GameState state) override;
    
    qreal getX() const { return x; }
    qreal getY() const { return y; }
    int getRow() const { return row; }
    bool isDead() const { return health <= 0; }
    
    void takeDamage(const Core::Damage& damage);
    bool isEating() const { return eating; }
    void setEating(bool eat) { eating = eat; }

protected:
    qreal x, y;
    int row;
    qreal speed;
    int health;
    int attack_power;
    int attack_interval;
    int attack_ticks;
    
    bool poison;
    bool slowness;
    bool onfire;
    bool eating;
    
    Core::AnimationUnit* aniUnit;
    Core::Protection protection;
};

}; };

#endif // ZOMBIE_BASE_H
