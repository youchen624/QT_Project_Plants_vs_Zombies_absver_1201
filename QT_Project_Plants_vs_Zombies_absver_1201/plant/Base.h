#ifndef BASE_H
#define BASE_H

#include "core/ObjectTickable.h"

namespace PVZ{
namespace Plant {

class Base : public Core::ObjectTickable
{
    Q_OBJECT
public:
    Base(Core::GameCore* core);
    virtual ~Base();

    bool eatable() {
        return this->zombieEatable;
    }
    
    int getHealth() const { return health; }
    void takeDamage(int amount) { health -= amount; }
    bool isDead() const { return health <= 0; }

protected:
    bool zombieEatable;
    int price;
    int health;
};

};
};

#endif // BASE_H
