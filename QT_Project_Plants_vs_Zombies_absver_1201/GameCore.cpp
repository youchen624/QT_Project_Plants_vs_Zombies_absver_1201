#include "GameCore.h"
#include <QDebug>
#include <QGraphicsView>
#include <QDateTime>
#include <QTimer>
using namespace PVZ::Core;

GameCore::GameCore(QObject *parent, bool debug) : QObject(parent), _debug(debug) {
    this->gameState = GameState::Pause;
    this->scene.setSceneRect(0, 0, 800, 600);
    this->view = new QGraphicsView(&this->scene);
    view->setWindowTitle("PVZ");
    view->resize(800, 600);
    view->show();
    connect(
        &timer,
        &QTimer::timeout,
        this,
        &GameCore::clock
    );
};

void GameCore::start() {
    this->timer.setInterval(TICK_PER_MS);
    this->timer.start();

    this->gameState = GameState::Progressing;
    qDebug() << "Game starting.\n";
};

void GameCore::stop() {};

void GameCore::gameover() {};


int GameCore::firstPlant_x(int y) {
    for (int i = plantsGrid[y].length() - 1; i >= 0; --i) {
        for (int ii = 0; ii < plantsGrid[y][i].length(); ++ii) {
            if (plantsGrid[y][i][ii] && plantsGrid[y][i][ii]->eatable())
                return i;
        }
    }
    return -1;
};

void GameCore::clock() {
    if (gameState == GameState::Pause) return;
    if (_debug) {
        dateTime = QDateTime::currentDateTime();
        qDebug()
            << dateTime.toString()
        << "ticked\n";
    }
    emit tick(gameState);
};
