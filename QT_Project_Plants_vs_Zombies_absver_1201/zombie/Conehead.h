#ifndef ZOMBIE_CONEHEAD_H
#define ZOMBIE_CONEHEAD_H

#include "zombie/Base.h"

namespace PVZ {
namespace Zombie {

class Conehead : public Base
{
    Q_OBJECT
public:
    Conehead(Core::GameCore* core, int row);
    ~Conehead();

protected:
    const QList<QPixmap>& frames() const;
    const QHash<QString, Core::P2>& states() const;
};

}; };

#endif // ZOMBIE_CONEHEAD_H
