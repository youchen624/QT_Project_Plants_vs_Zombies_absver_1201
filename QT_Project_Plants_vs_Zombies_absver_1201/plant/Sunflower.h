#ifndef SUNFLOWER_H
#define SUNFLOWER_H

#include "plant/Base.h"
#include "core/AnimationUnit.h"

namespace PVZ {
namespace Plant {

class Sunflower : public Base
{
public:
    static constexpr int SUN_VALUE = 50;
    static constexpr int SUN_SPAWM_TICKS = 100;

    static QHash<QString, Core::P2>& motionStates_sunflower;

public:
    Sunflower(Core::GameCore* core);
    ~Sunflower();

    void tick(Core::GameState state) override;

protected:
    const QList<QPixmap>& frames() const;
    const QHash<QString, Core::P2>& states() const;
    Core::AnimationUnit aniUnit;

private:
    int sun_ticks;
};

};
};

/*
namespace PVZ{
namespace Plant {

class Base : public Core::ObjectTickable
{
public:
    Base(Core::GameCore* core);
    ~Base() {};

    bool eatable() {
        return this->zombieEatable;
    }

protected:
    bool zombieEatable;
    int price;
    int health;
};

};
};
*/


#endif // SUNFLOWER_H
