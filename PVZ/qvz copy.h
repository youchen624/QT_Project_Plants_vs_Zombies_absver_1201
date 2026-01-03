#ifndef QVZ_H
#define QVZ_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QMessageBox>
#include "mainmenu.h"
#include "levelselect.h"
#include "plantselect.h"
#include "gamescene.h"

class QVZ : public QMainWindow
{
    Q_OBJECT

public:
    QVZ(QWidget *parent = nullptr);
    ~QVZ();

private slots:
    void showMainMenu();
    void showLevelSelect();
    void showPlantSelect(int levelId);
    void showGameScene();
    void showGameSceneWithLevel(int levelId);
    void showGameSceneWithPlants(QVector<PlantCard::PlantType> plants);
    void showOptions();
    void exitApplication();
    void onGameWon();
    void onGameLost();

private:
    QStackedWidget *m_stackedWidget;
    MainMenu *m_mainMenu;
    LevelSelect *m_levelSelect;
    PlantSelect *m_plantSelect;
    GameScene *m_gameScene;
    int m_selectedLevelId;
};
#endif // QVZ_H
