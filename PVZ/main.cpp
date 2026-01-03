#include "qvz.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QVZ w;
    w.show();
    return a.exec();
}
