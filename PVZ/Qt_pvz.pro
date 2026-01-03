QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    qvz.cpp \
    entity.cpp \
    plants/plantbase.cpp \
    plants/shooterplant.cpp \
    plants/sunflowerplant.cpp \
    plants/defensiveplant.cpp \
    plants/bombplant.cpp \
    plants/skeletontreeplant.cpp \
    zombies/zombiebase.cpp \
    zombies/zombietypes.cpp \
    projectile.cpp \
    gamescene.cpp \
    mainmenu.cpp \
    levelselect.cpp \
    plantcard.cpp \
    sun.cpp \
    pausemenu.cpp \
    optionsmenu.cpp \
    plantselect.cpp \
    leaderboardmanager.cpp \
    leaderboarddialog.cpp \
    playernamedialog.cpp

HEADERS += \
    qvz.h \
    entity.h \
    plants/plantbase.h \
    plants/shooterplant.h \
    plants/sunflowerplant.h \
    plants/defensiveplant.h \
    plants/bombplant.h \
    plants/skeletontreeplant.h \
    plants/plant.h \
    zombies/zombiebase.h \
    zombies/zombietypes.h \
    zombies/zombie.h \
    projectile.h \
    gamescene.h \
    mainmenu.h \
    levelselect.h \
    plantcard.h \
    sun.h \
    pausemenu.h \
    optionsmenu.h \
    plantselect.h \
    leaderboardmanager.h \
    leaderboarddialog.h \
    playernamedialog.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
