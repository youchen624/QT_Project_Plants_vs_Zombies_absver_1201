#include <QCoreApplication>
#include <QDebug>
#include "game/Game.h"
#include "plants/Sunflower.h"
#include "plants/Peashooter.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    qDebug() << "=== Testing Plant Placement ===";
    
    Game game;
    
    qDebug() << "Initial state:";
    qDebug() << "  Sun points:" << game.getSunPoints();
    qDebug() << "  Grid size:" << game.getGridRows() << "x" << game.getGridCols();
    
    qDebug() << "\nStarting game...";
    game.start();
    
    qDebug() << "After start:";
    qDebug() << "  Sun points:" << game.getSunPoints();
    qDebug() << "  Running:" << game.isRunning();
    
    qDebug() << "\nTesting Sunflower placement at (2, 1)...";
    Sunflower* sunflower = new Sunflower();
    qDebug() << "  Sunflower cost:" << sunflower->getCost();
    qDebug() << "  Image null?" << sunflower->getImage().isNull();
    qDebug() << "  Image size:" << sunflower->getImage().size();
    
    bool success1 = game.placePlant(sunflower, 2, 1);
    qDebug() << "  Placement result:" << (success1 ? "SUCCESS" : "FAILED");
    qDebug() << "  Remaining sun:" << game.getSunPoints();
    
    if (!success1) {
        delete sunflower;
    }
    
    qDebug() << "\nTesting Peashooter placement at (2, 2)...";
    Peashooter* peashooter = new Peashooter();
    qDebug() << "  Peashooter cost:" << peashooter->getCost();
    qDebug() << "  Image null?" << peashooter->getImage().isNull();
    qDebug() << "  Image size:" << peashooter->getImage().size();
    
    bool success2 = game.placePlant(peashooter, 2, 2);
    qDebug() << "  Placement result:" << (success2 ? "SUCCESS" : "FAILED");
    qDebug() << "  Remaining sun:" << game.getSunPoints();
    
    if (!success2) {
        delete peashooter;
    }
    
    qDebug() << "\n=== Test Complete ===";
    qDebug() << "Sunflower placed:" << success1;
    qDebug() << "Peashooter placed:" << success2;
    
    return 0;
}
