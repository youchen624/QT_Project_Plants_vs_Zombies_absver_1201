QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    core/AnimationUnit.cpp \
    core/Bullet.cpp \
    core/GameCore.cpp \
    core/Global.cpp \
    core/ObjectTickable.cpp \
    core/Sun.cpp \
    main.cpp \
    plant/Base.cpp \
    plant/Peashooter.cpp \
    plant/Sunflower.cpp \
    plant/Wallnut.cpp \
    zombie/Base.cpp \
    zombie/Conehead.cpp \
    zombie/General.cpp

HEADERS += \
    core/AnimationUnit.h \
    core/Bullet.h \
    core/GameCore.h \
    core/Global.h \
    core/ObjectTickable.h \
    core/Sun.h \
    plant/Base.h \
    plant/Peashooter.h \
    plant/Sunflower.h \
    plant/Wallnut.h \
    zombie/Base.h \
    zombie/Conehead.h \
    zombie/General.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES +=
