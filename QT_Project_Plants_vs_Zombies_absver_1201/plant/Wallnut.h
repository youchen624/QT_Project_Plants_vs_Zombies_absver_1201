#ifndef WALLNUT_H
#define WALLNUT_H

#include "plant/Base.h"
#include "core/AnimationUnit.h"

namespace PVZ {
namespace Plant {

class Wallnut : public Base
{
    Q_OBJECT
public:
    Wallnut(Core::GameCore* core, int row, int col);
    ~Wallnut();

    void tick(Core::GameState state) override;

protected:
    const QList<QPixmap>& frames() const;
    const QHash<QString, Core::P2>& states() const;
    Core::AnimationUnit* aniUnit;

private:
    int row;
    int col;
};

}; };

#endif // WALLNUT_H
