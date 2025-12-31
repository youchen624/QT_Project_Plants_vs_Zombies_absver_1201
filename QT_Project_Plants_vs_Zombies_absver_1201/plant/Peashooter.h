#ifndef PEASHOOTER_H
#define PEASHOOTER_H

#include "plant/Base.h"
#include "core/AnimationUnit.h"

namespace PVZ {
namespace Plant {

class Peashooter : public Base
{
    Q_OBJECT
public:
    static constexpr int SHOOT_INTERVAL_TICKS = 30;

public:
    Peashooter(Core::GameCore* core, int row, int col);
    ~Peashooter();

    void tick(Core::GameState state) override;

protected:
    const QList<QPixmap>& frames() const;
    const QHash<QString, Core::P2>& states() const;
    Core::AnimationUnit* aniUnit;

private:
    int shoot_ticks;
    int row;
    int col;
};

}; };

#endif // PEASHOOTER_H
