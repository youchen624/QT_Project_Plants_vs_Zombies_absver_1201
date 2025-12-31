#include "Conehead.h"
#include "core/GameCore.h"

using namespace PVZ::Zombie;

const QList<QPixmap>& Conehead::frames() const {
    static const QList<QPixmap> data = []() {
        QList<QPixmap> list;
        // Use a placeholder for now - darker color to distinguish
        QPixmap zombie_img(60, 80);
        zombie_img.fill(QColor(150, 100, 50));
        list.append(zombie_img);
        return list;
    }();
    return data;
}

const QHash<QString, PVZ::Core::P2>& Conehead::states() const {
    static const QHash<QString, PVZ::Core::P2> data = []() {
        QHash<QString, PVZ::Core::P2> hash;
        hash.insert("Walk", PVZ::Core::P2(0, 0));
        return hash;
    }();
    return data;
}

Conehead::Conehead(Core::GameCore* core, int row) : Base(core, row) {
    health = 370;  // More health than regular zombie
    speed = 0.5;
    
    // Add cone protection
    protection.type = Core::Protection::Type::Plastic;
    protection.value = 370;
    
    aniUnit = new Core::AnimationUnit(core, frames(), states());
    aniUnit->changeMotionState("Walk");
    aniUnit->setPosition(x, y);
}

Conehead::~Conehead() {
}
