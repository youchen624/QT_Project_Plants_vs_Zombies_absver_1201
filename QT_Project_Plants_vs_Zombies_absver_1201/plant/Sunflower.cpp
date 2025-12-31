#include "plant/Sunflower.h"
#include "core/GameCore.h"
#include "core/Sun.h"

using namespace PVZ::Plant;

const QList<QPixmap>& Sunflower::frames() const {
    static const QList<QPixmap> data = []() {
        QList<QPixmap> list;
        list.append(QPixmap(":/.resources/img/plant/sunflower.png"));
        return list;
    }();
    return data;
}

const QHash<QString, PVZ::Core::P2>& Sunflower::states() const {
    static const QHash<QString, PVZ::Core::P2> data = []() {
        QHash<QString, PVZ::Core::P2> hash;
        hash.insert("Idle", PVZ::Core::P2(0, 0));
        return hash;
    }();
    return data;
}

Sunflower::Sunflower(Core::GameCore* core, int row, int col) :
    Base(core), sun_ticks(0), row(row), col(col)
{
    zombieEatable = true;
    price = 50;
    health = 300;

    aniUnit = new Core::AnimationUnit(core, frames(), states());
    aniUnit->changeMotionState("Idle");
    
    // Position the plant on the grid using helper functions
    aniUnit->setPosition(getGridX(col), getGridY(row));
}

Sunflower::~Sunflower() {
    delete aniUnit;
}

void Sunflower::tick(Core::GameState state) {
    if (state != Core::GameState::Progressing) return;
    
    aniUnit->update();

    // Produce sun
    if (++sun_ticks >= SUN_SPAWN_TICKS) {
        core->spawnSun(getGridX(col, 30), getGridY(row, 30), false);
        qDebug() << "Sunflower produced sun";
        
        sun_ticks = 0;
    }
}
