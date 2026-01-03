#include "qvz.h"
#include "leaderboarddialog.h"
#include <QApplication>

QVZ::QVZ(QWidget *parent)
    : QMainWindow(parent)
    , m_selectedLevelId(1)
{
    setWindowTitle("Plants vs Zombies - Abstration Version");
    setMinimumSize(900, 600);

    // Create stacked widget for switching between screens
    m_stackedWidget = new QStackedWidget(this);
    setCentralWidget(m_stackedWidget);

    // Create main menu
    m_mainMenu = new MainMenu(this);
    connect(m_mainMenu, &MainMenu::selectLevel, this, &QVZ::showLevelSelect);
    connect(m_mainMenu, &MainMenu::showLeaderboard, this, &QVZ::showLeaderboard);
    connect(m_mainMenu, &MainMenu::exitGame, this, &QVZ::exitApplication);

    // Create level select screen
    m_levelSelect = new LevelSelect(this);
    connect(m_levelSelect, &LevelSelect::levelSelected, this, &QVZ::showPlantSelect);
    connect(m_levelSelect, &LevelSelect::backToMenu, this, &QVZ::showMainMenu);

    // Create plant select screen
    m_plantSelect = new PlantSelect(this);
    connect(m_plantSelect, &PlantSelect::plantsSelected, this, &QVZ::showGameSceneWithPlants);
    connect(m_plantSelect, &PlantSelect::backToMenu, this, &QVZ::showLevelSelect);

    // Create game scene
    m_gameScene = new GameScene(this);
    connect(m_gameScene, &GameScene::gameWon, this, &QVZ::onGameWon);
    connect(m_gameScene, &GameScene::gameLost, this, &QVZ::onGameLost);
    connect(m_gameScene, &GameScene::exitToMainMenu, this, &QVZ::showMainMenu);

    // Add widgets to stack
    m_stackedWidget->addWidget(m_mainMenu);
    m_stackedWidget->addWidget(m_levelSelect);
    m_stackedWidget->addWidget(m_plantSelect);
    m_stackedWidget->addWidget(m_gameScene);

    // Show main menu by default
    showMainMenu();
}

QVZ::~QVZ() 
{
}

void QVZ::showMainMenu()
{
    m_stackedWidget->setCurrentWidget(m_mainMenu);
}

void QVZ::showLevelSelect()
{
    m_stackedWidget->setCurrentWidget(m_levelSelect);
}

void QVZ::showPlantSelect(int levelId)
{
    m_selectedLevelId = levelId;
    m_stackedWidget->setCurrentWidget(m_plantSelect);
}

void QVZ::showGameScene()
{
    // Quick start with level 1 and default plants
    showGameSceneWithLevel(1);
}

void QVZ::showGameSceneWithLevel(int levelId)
{
    m_stackedWidget->setCurrentWidget(m_gameScene);
    m_gameScene->startGame(levelId);
}

void QVZ::showGameSceneWithPlants(QVector<PlantCard::PlantType> plants)
{
    m_stackedWidget->setCurrentWidget(m_gameScene);
    m_gameScene->setSelectedPlants(plants);
    m_gameScene->startGame(m_selectedLevelId);
}

void QVZ::showOptions()
{
    // Show in-game options menu in the game scene
    if (m_gameScene && m_gameScene->isVisible()) {
        m_gameScene->showOptionsMenu();
    } else {
        // If not in game, show a simple message
        QMessageBox::information(this, "選項",
            "功能:\n"
            "- 自動收集陽光\n"
            "- 倍速 (1x, 3x, 5x)");
    }
}

void QVZ::showLeaderboard()
{
    LeaderboardDialog dialog(this);
    dialog.exec();
}

void QVZ::exitApplication()
{
    QApplication::quit();
}

void QVZ::onGameWon()
{
    QMessageBox::information(this, "勝利",
        "你的植物成功守住所有殭屍了!!");
    showMainMenu();
}

void QVZ::onGameLost()
{
    QMessageBox::information(this, "失敗",
        "殭屍入侵你家了!!!");
    showMainMenu();
}
