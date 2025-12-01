#ifndef ITEMRENDERABLE_H
#define ITEMRENDERABLE_H
#include <QGraphicsItem>

namespace PVZ {
namespace Core {};
namespace Object {
    class ItemRenderable
    {
    public:
        ItemRenderable();
        virtual QGraphicsItem* getGraphicsItem() = 0;
    };
};
};

#endif // ITEMRENDERABLE_H
