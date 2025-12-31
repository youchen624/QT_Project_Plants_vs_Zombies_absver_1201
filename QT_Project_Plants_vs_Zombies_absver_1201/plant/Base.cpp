#include "Base.h"
// #include "core/GameCore.h"

using namespace PVZ::Plant;

Base::Base(Core::GameCore* core) :
    Core::ObjectTickable(core),
    zombieEatable(true) {
};

Base::~Base() {};
