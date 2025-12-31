#include "AnimationUnit.h"
#include "GameCore.h"

using namespace PVZ::Core;

AnimationUnit::AnimationUnit(GameCore* core,
                             const QList<QPixmap>& frames,
                             const QHash<QString, P2>& states) :
    c_frame(0), core(core), frames(frames), motionStates(states) {
    graphic_main = new QGraphicsPixmapItem();
};

AnimationUnit::~AnimationUnit() {
    delete graphic_main;
};


void AnimationUnit::changeMotionState(QString state) {
    if (!motionStates.contains(state)) return;
    c_state = state;
    update();
};

void AnimationUnit::update() {
    // if (!core) return;
    P2 p = motionStates.value(c_state);
    if (p.out_range(++c_frame)) c_frame = p.a;
    QPixmap pix = frames[c_frame];
    // core->m_scene->addItem(pix);
    graphic_main->setPixmap(pix);
};
