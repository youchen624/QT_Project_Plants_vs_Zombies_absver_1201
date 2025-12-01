#include "ItemTickable.h"

using namespace PVZ;
using namespace Object;

ItemTickable::ItemTickable() {
    pause = false;
}

void ItemTickable::tick(Core::GameCore* core) {};
