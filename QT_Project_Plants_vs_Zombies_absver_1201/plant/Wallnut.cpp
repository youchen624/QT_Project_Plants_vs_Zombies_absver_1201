#include "plant/Wallnut.h"
#include "core/GameCore.h"

using namespace PVZ::Plant;

const QList<QPixmap>& Wallnut::frames() const {
    static const QList<QPixmap> data = []() {
        QList<QPixmap> list;
        QPixmap img(":/.resources/img/plant/stalk.png");
        list.append(img);
        return list;
    }();
    return data;
}

const QHash<QString, PVZ::Core::P2>& Wallnut::states() const {
    static const QHash<QString, PVZ::Core::P2> data = []() {
        QHash<QString, PVZ::Core::P2> hash;
        hash.insert("Idle", PVZ::Core::P2(0, 0));
        return hash;
    }();
    return data;
}

Wallnut::Wallnut(Core::GameCore* core, int row, int col) :
    Base(core), row(row), col(col)
{
    zombieEatable = true;
    price = 50;
    health = 4000;  // High health for defensive plant

    aniUnit = new Core::AnimationUnit(core, frames(), states());
    aniUnit->changeMotionState("Idle");
    
    // Position the plant on the grid using helper functions
    aniUnit->setPosition(getGridX(col), getGridY(row));
}

Wallnut::~Wallnut() {
    delete aniUnit;
}

void Wallnut::tick(Core::GameState state) {
    if (state != Core::GameState::Progressing) return;
    
    aniUnit->update();
    
    // Wallnut doesn't do anything special, just provides defense
}
