#ifndef BASEPLANT_H
#define BASEPLANT_H

#include "ItemTickable.h"

namespace PVZ {
namespace Object {};
namespace Plant {
class BasePlant : public Object::ItemTickable
    {
    public:
        BasePlant();
        ~BasePlant();

        bool eatable() {
            return this->zombieEatable;
        };

    protected:
        bool zombieEatable;
        int price;

        int health;
    };
};
};

#endif // BASEPLANT_H
