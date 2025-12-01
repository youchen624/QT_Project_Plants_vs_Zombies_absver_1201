#include "widget.h"
#include "GameCore.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PVZ::Core::GameCore game(nullptr, true);
    game.start();
    // Widget w;
    // w.show();
    return a.exec();
}
