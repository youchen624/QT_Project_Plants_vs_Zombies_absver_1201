#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsView>
#include "Game.h"
#include "GameScene.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onGameStarted();
    void onGameStopped();
    void onSunPointsChanged(int amount);
    void onTickUpdate(int tickCount);
    void startButtonClicked();
    void pauseButtonClicked();
    void testPlacePlants();
    void testSpawnZombies();
    void selectSunflower();
    void selectPeashooter();

private:
    void setupUI();
    void updateStatusLabel();

    Game* game;
    GameScene* gameScene;
    QGraphicsView* gameView;
    QLabel* statusLabel;
    QLabel* sunPointsLabel;
    QLabel* tickLabel;
    QPushButton* startButton;
    QPushButton* pauseButton;
    QPushButton* testPlantsButton;
    QPushButton* testZombiesButton;
    QPushButton* sunflowerButton;
    QPushButton* peashooterButton;
};
#endif // MAINWINDOW_H
