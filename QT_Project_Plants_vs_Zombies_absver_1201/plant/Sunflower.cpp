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
}

Sunflower::~Sunflower() {
    delete aniUnit;
}

void Sunflower::tick(Core::GameState state) {
    if (state != Core::GameState::Progressing) return;
    
    aniUnit->update();

    // Produce sun
    if (++sun_ticks >= SUN_SPAWN_TICKS) {
        qreal x = 250 + col * 100 + 30;  // GRID_START_X = 250, GRID_SIZE = 100
        qreal y = 80 + row * 100 + 30;   // GRID_START_Y = 80
        
        core->spawnSun(x, y, false);
        qDebug() << "Sunflower produced sun at" << x << "," << y;
        
        sun_ticks = 0;
    }
}
