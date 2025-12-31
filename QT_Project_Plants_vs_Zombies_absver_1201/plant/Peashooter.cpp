#include "plant/Peashooter.h"
#include "core/GameCore.h"
#include "core/Bullet.h"

using namespace PVZ::Plant;

const QList<QPixmap>& Peashooter::frames() const {
    static const QList<QPixmap> data = []() {
        QList<QPixmap> list;
        QPixmap img(":/.resources/img/plant/leave.png");
        list.append(img);
        return list;
    }();
    return data;
}

const QHash<QString, PVZ::Core::P2>& Peashooter::states() const {
    static const QHash<QString, PVZ::Core::P2> data = []() {
        QHash<QString, PVZ::Core::P2> hash;
        hash.insert("Idle", PVZ::Core::P2(0, 0));
        return hash;
    }();
    return data;
}

Peashooter::Peashooter(Core::GameCore* core, int row, int col) :
    Base(core), shoot_ticks(0), row(row), col(col)
{
    zombieEatable = true;
    price = 100;
    health = 300;

    aniUnit = new Core::AnimationUnit(core, frames(), states());
    aniUnit->changeMotionState("Idle");
    
    // Position the plant on the grid using helper functions
    aniUnit->setPosition(getGridX(col), getGridY(row));
}

Peashooter::~Peashooter() {
    delete aniUnit;
}

void Peashooter::tick(Core::GameState state) {
    if (state != Core::GameState::Progressing) return;
    
    aniUnit->update();

    // Shoot peas periodically
    if (++shoot_ticks >= SHOOT_INTERVAL_TICKS) {
        // Create a bullet at the plant's position with offset for shooting point
        new Core::Bullet(core, getGridX(col, 50), getGridY(row, 40), row);
        
        shoot_ticks = 0;
    }
}
