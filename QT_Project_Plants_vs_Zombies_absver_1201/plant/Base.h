#ifndef BASE_H
#define BASE_H

#include "core/ObjectTickable.h"

namespace PVZ{
namespace Plant {

class Base : public Core::ObjectTickable
{
public:
    Base(Core::GameCore* core);
    ~Base();

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

#endif // BASE_H
