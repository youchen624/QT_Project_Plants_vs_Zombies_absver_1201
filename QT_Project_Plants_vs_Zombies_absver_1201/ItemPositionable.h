#ifndef ITEMPOSITIONABLE_H
#define ITEMPOSITIONABLE_H

#include <QPointF>

namespace PVZ {
namespace Object {
    class ItemPositionable
    {
    public:
        ItemPositionable();

        bool x_collision();
    private:
        QPointF pos;
    };
};
};

#endif // ITEMPOSITIONABLE_H
