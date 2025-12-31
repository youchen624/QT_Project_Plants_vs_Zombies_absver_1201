#ifndef ANIMATIONUNIT_H
#define ANIMATIONUNIT_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QHash>


namespace PVZ {
namespace Core {
    class GameCore;

    typedef struct P2 {
        P2(int a, int b) : a(a), b(b) {};
        bool out_range(int c) {
            return !((c >= a) || (c <= b));
        };
        int a;
        int b;
    } P2;

    class AnimationUnit
    {
    /*
     * AnimationUnit
     // * MotionState
    */
    public:
        // class MotionState {};
    public:
        AnimationUnit(GameCore* core,
                      const QList<QPixmap>& frames,
                      const QHash<QString, P2>& states);
        ~AnimationUnit();

        void setPosition();
        void setPosition(qreal x, qreal y);

        // virtual
        void update();

        void changeMotionState(QString state);
    private:
        QString c_state;
        int c_frame;

        QGraphicsPixmapItem* graphic_main;

        GameCore* core;
        const QList<QPixmap>& frames;
        const QHash<QString, P2>& motionStates;
        // static QHash<QString, P> motionStates; // key : value
        /*
            motionStates = {
                {"Walk", {1, 5}},
                {"Jog",  {6, 10}}
            };
        */
    };

};
};

#endif // ANIMATIONUNIT_H
