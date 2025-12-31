#include "ObjectTickable.h"
#include "core/GameCore.h"
using namespace PVZ::Core;

ObjectTickable::ObjectTickable(GameCore* core) : core(core), pause(false) {
    connect(core, &GameCore::coreTick, this, [this]() {
        this->tick(core->state);
    });
};

void ObjectTickable::tick(GameState state) {
    // Base implementation - override in derived classes
};
