#ifndef GLOBAL_H
#define GLOBAL_H

#include <QPixmap>
#include <QString>

namespace PVZ::Core {
class GameCore;

// get image with cache system
QPixmap img(const QString &path);

// 遊戲模式
enum class GameState {
    Pause = 0,
    Progressing,
    GameOver_lose,
    GameOver_win
};

// 傷害obj
typedef struct Damage {
    quint64 value;
    enum class Type {
        Normal = 0,
        Explode,    // 爆炸
        Bite,       // 啃咬
        Burn,       // 燃燒
        Poison,     // 毒藥
        Drown,      // 溺水
        Force       // 原力
    } type;
} Damage;

// 防禦obj
typedef struct Protection {
    enum class Type {
        Normal,
        Plastic,
        Metal
    } type;

    quint64 value;
} Protection;

// 武器
typedef struct Weapon {
    enum class Type {
        Normal,
        Metal
    };
    virtual void attack(GameCore*) {};

    quint64 boosted;
} Weapon;

};

#endif // GLOBAL_H
