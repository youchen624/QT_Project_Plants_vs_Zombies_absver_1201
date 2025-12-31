#include "plant/Sunflower.h"
#include "core/Global.h"

namespace PVZ::Core {
class GameCore;
enum class GameState;
};

using namespace PVZ::Plant;

const QList<QPixmap>& Sunflower::frames() const {
    static const QList<QPixmap> data = []() {
        QList<QPixmap> list;
        list.append(QPixmap(":/img/plant/sunflower.png"));
        // 在這裡加載圖片，例如使用你之前的大圖切分邏輯
        // list.append(...);
        return list;
    }();
    return data;
};
const QHash<QString, PVZ::Core::P2>& Sunflower::states() const {
    static const QHash<QString, PVZ::Core::P2> data = []() {
        QHash<QString, PVZ::Core::P2> hash;
        hash.insert("Idle", {1, 1});
        return hash;
    }();
    return data;
};

Sunflower::Sunflower(Core::GameCore* core) :
    Base(core),
    aniUnit(core, frames(), states()),
    sun_ticks(0) {
    zombieEatable = true;
    price = 50;
    health = 300;

    aniUnit.changeMotionState("Idle");
    // connect(core, &Core::GameCore::coreTick, this, &Sunflower::tick);
};

Sunflower::~Sunflower() {};

void Sunflower::tick(Core::GameState state) {
    if (state != Core::GameState::Progressing) return;
    aniUnit.update();

    // 生產陽光
    if (++sun_ticks >= SUN_SPAWM_TICKS) {
        qDebug() << "Sun spawmed.";
        sun_ticks = 0;
    }
};
