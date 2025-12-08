#ifndef ITEMTICKABLE_H
#define ITEMTICKABLE_H

// #include "GameCore.h"

namespace PVZ {
namespace Core {
    class GameCore;
    enum class GameState;
};
namespace Object {
    class ItemTickable
    {
    public:
        ItemTickable(Core::GameCore* core);
        virtual void tick(Core::GameState);
    protected:
        bool pause;
    };
};
};

#endif // ITEMTICKABLE_H
