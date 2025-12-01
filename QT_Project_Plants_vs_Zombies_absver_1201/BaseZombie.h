#ifndef BASEZOMBIE_H
#define BASEZOMBIE_H

#include "ItemTickable.h"
#include "ItemRenderable.h"

namespace PVZ {
namespace Object {
    class BaseZombie : public ItemTickable
    {
    public:
        BaseZombie();
    protected:
        bool posion;
        bool slowness;
        bool onfire;

        bool eating;
    };
};
};

#endif // BASEZOMBIE_H
