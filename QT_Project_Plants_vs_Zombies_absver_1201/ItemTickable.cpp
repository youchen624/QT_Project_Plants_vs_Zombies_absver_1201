#include "ItemTickable.h"
#include "GameCore.h"

using namespace PVZ;
using namespace Object;

ItemTickable::ItemTickable(Core::GameCore* core) : pause(false) {
    // connect(
    //     core,
    //     &Core::GameCore::tick,
    //     this,
    //     &ItemTickable::tick
    // );
};

void ItemTickable::tick(Core::GameState) {};
