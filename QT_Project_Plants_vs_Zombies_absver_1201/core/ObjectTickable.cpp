#include "ObjectTickable.h"
#include "core/GameCore.h"
using namespace PVZ::Core;

ObjectTickable::ObjectTickable(GameCore* core) : core(core) {

    connect(core, &GameCore::coreTick(GameState), this, &ObjectTickable::tick(GameState));
    // connect(
    //     core,
    //     &Core::GameCore::tick,
    //     this,
    //     &ItemTickable::tick
    // );
};

void ObjectTickable::tick(GameState state) {};
