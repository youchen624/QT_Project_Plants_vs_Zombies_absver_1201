#ifndef OBJECTTICKABLE_H
#define OBJECTTICKABLE_H

#include <QObject>

namespace PVZ {
namespace Core {
    class GameCore;
    enum class GameState;

class ObjectTickable : public QObject
{
    Q_OBJECT
public:
    ObjectTickable(GameCore* core);
    virtual void tick(GameState state);

protected:
    bool pause;

    GameCore* core;
};

};};
#endif // OBJECTTICKABLE_H
