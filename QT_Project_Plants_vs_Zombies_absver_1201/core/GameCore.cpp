#include "GameCore.h"
using namespace PVZ::Core;

GameCore::GameCore() : sflower(Plant::Sunflower(this)), state(GameState::Progressing)
{

    // this->setWindowTitle(tr("Plants vs Zombies (abs-ver)"));
    m_scene = new QGraphicsScene();
    m_view  = new QGraphicsView(m_scene);
    m_view->setWindowTitle(tr("Plants vs Zombies (abs-ver)"));

    m_view->show();
}

QPixmap Ground::pix = QPixmap(":/img/background/1");

Ground::Ground(GameCore* core) : core(core) {
    graphic_main.setPixmap(pix);
    core->m_scene->addItem(&graphic_main);
    graphic_main.setPos(0, 0);
};
