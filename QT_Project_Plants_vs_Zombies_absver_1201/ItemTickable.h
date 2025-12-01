#ifndef ITEMTICKABLE_H
#define ITEMTICKABLE_H

#include "GameCore.h"

namespace PVZ {
namespace Core {};
namespace Object {
    class ItemTickable
    {
    public:
        ItemTickable();
        virtual void tick(Core::GameCore* core);
    protected:
        bool pause;
    };
};
};

#endif // ITEMTICKABLE_H
