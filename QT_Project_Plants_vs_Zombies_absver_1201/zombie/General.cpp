#include "General.h"
#include "core/GameCore.h"

using namespace PVZ::Zombie;

const QList<QPixmap>& General::frames() const {
    static const QList<QPixmap> data = []() {
        QList<QPixmap> list;
        // Use a placeholder for now
        QPixmap zombie_img(60, 80);
        zombie_img.fill(QColor(100, 100, 100));
        list.append(zombie_img);
        return list;
    }();
    return data;
}

const QHash<QString, PVZ::Core::P2>& General::states() const {
    static const QHash<QString, PVZ::Core::P2> data = []() {
        QHash<QString, PVZ::Core::P2> hash;
        hash.insert("Walk", PVZ::Core::P2(0, 0));
        return hash;
    }();
    return data;
}

General::General(Core::GameCore* core, int row) : Base(core, row) {
    health = 200;
    speed = 0.5;
    
    aniUnit = new Core::AnimationUnit(core, frames(), states());
    aniUnit->changeMotionState("Walk");
    aniUnit->setPosition(x, y);
}

General::~General() {
}
