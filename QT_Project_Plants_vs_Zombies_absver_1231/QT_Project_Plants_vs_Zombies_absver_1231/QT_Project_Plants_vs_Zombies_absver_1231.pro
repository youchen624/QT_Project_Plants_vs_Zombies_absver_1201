QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    ui/mainwindow.cpp \
    entities/Plant.cpp \
    entities/Zombie.cpp \
    game/PlantCell.cpp \
    plants/Sunflower.cpp \
    plants/Peashooter.cpp \
    zombies/NormalZombie.cpp \
    game/Game.cpp

HEADERS += \
    ui/mainwindow.h \
    entities/Plant.h \
    entities/Zombie.h \
    game/PlantCell.h \
    plants/Sunflower.h \
    plants/Peashooter.h \
    zombies/NormalZombie.h \
    game/Game.h

INCLUDEPATH += entities plants zombies game ui

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
