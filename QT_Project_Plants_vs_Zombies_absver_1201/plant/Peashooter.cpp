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
    
    // Position the plant on the grid
    qreal x = 250 + col * 100;
    qreal y = 80 + row * 100;
    // Note: AnimationUnit needs a setPosition method implementation
}

Peashooter::~Peashooter() {
    delete aniUnit;
}

void Peashooter::tick(Core::GameState state) {
    if (state != Core::GameState::Progressing) return;
    
    aniUnit->update();

    // Shoot peas periodically
    if (++shoot_ticks >= SHOOT_INTERVAL_TICKS) {
        qreal x = 250 + col * 100 + 50;
        qreal y = 80 + row * 100 + 40;
        
        // Create a bullet
        new Core::Bullet(core, x, y, row);
        
        shoot_ticks = 0;
    }
}
