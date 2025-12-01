#include "GameCore.h"
#include <QDebug>
#include <QGraphicsView>
#include <QDateTime>
#include <QTimer>
using namespace PVZ::Core;

GameCore::GameCore(QObject *parent, bool debug) : QObject(parent), _debug(debug) {
    this->pause = true;
    this->scene.setSceneRect(0, 0, 800, 600);
    this->view = new QGraphicsView(&this->scene);
    view->setWindowTitle("PVZ");
    view->resize(800, 600);
    view->show();
    connect(
        &timer,
        &QTimer::timeout,
        this,
        &GameCore::tick
    );
};

void GameCore::start() {
    this->timer.setInterval(TICK_PER_MS);
    this->timer.start();

    this->pause = false;
    qDebug() << "Game starting.\n";
};

void GameCore::stop() {};

void GameCore::gameover() {};


int GameCore::firstPlant_x(int y) {};

void GameCore::tick() {
    if (pause) return;
    if (_debug) {
        dateTime = QDateTime::currentDateTime();
        qDebug()
            << dateTime.toString()
        << "ticked\n";
    }
};
