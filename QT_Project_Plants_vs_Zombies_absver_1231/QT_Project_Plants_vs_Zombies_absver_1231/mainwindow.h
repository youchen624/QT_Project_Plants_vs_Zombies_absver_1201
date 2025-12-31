#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include "Game.h"

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

private:
    void setupUI();
    void updateStatusLabel();

    Game* game;
    QLabel* statusLabel;
    QLabel* sunPointsLabel;
    QLabel* tickLabel;
    QPushButton* startButton;
    QPushButton* pauseButton;
    QPushButton* testPlantsButton;
    QPushButton* testZombiesButton;
};
#endif // MAINWINDOW_H
