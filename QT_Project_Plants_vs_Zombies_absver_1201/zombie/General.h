#ifndef ZOMBIE_GENERAL_H
#define ZOMBIE_GENERAL_H

#include "core/ObjectTickable.h"

namespace PVZ {
namespace Zombie {

class General : public Core::ObjectTickable
{
public:
    General(Core::GameCore* core);

protected:
    bool posion;
    bool slowness;
    bool onfire;

    bool eating;
};

};};
#endif // ZOMBIE_GENERAL_H
