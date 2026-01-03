#include "mainwindow.h"
#include "Sunflower.h"
#include "Peashooter.h"
#include "NormalZombie.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , game(new Game(this))
    , gameScene(new GameScene(game, this))
    , gameView(new QGraphicsView(gameScene, this))
    , statusLabel(new QLabel("Game Ready", this))
    , sunPointsLabel(new QLabel("Sun: 150", this))
    , tickLabel(new QLabel("Tick: 0", this))
    , startButton(new QPushButton("Start Game", this))
    , pauseButton(new QPushButton("Pause", this))
    , testPlantsButton(new QPushButton("Test: Place Plants", this))
    , testZombiesButton(new QPushButton("Test: Spawn Zombies", this))
    , sunflowerButton(new QPushButton("🌻 Sunflower (50)", this))
    , peashooterButton(new QPushButton("🌱 Peashooter (100)", this))
{
    setupUI();
    
    // Connect game signals
    connect(game, &Game::gameStarted, this, &MainWindow::onGameStarted);
    connect(game, &Game::gameStopped, this, &MainWindow::onGameStopped);
    connect(game, &Game::sunPointsChanged, this, &MainWindow::onSunPointsChanged);
    connect(game, &Game::tickUpdate, this, &MainWindow::onTickUpdate);
    
    // Connect button signals
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startButtonClicked);
    connect(pauseButton, &QPushButton::clicked, this, &MainWindow::pauseButtonClicked);
    connect(testPlantsButton, &QPushButton::clicked, this, &MainWindow::testPlacePlants);
    connect(testZombiesButton, &QPushButton::clicked, this, &MainWindow::testSpawnZombies);
    connect(sunflowerButton, &QPushButton::clicked, this, &MainWindow::selectSunflower);
    connect(peashooterButton, &QPushButton::clicked, this, &MainWindow::selectPeashooter);
    
    // Set window properties
    setWindowTitle("Plants vs. Zombies - QT Project");
    resize(800, 600);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    // Create central widget
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Create main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    // Status area
    QHBoxLayout* statusLayout = new QHBoxLayout();
    statusLayout->addWidget(statusLabel);
    statusLayout->addStretch();
    statusLayout->addWidget(sunPointsLabel);
    statusLayout->addWidget(tickLabel);
    mainLayout->addLayout(statusLayout);
    
    // Plant selection area
    QHBoxLayout* plantLayout = new QHBoxLayout();
    QLabel* plantLabel = new QLabel("Select Plant to Place:", this);
    plantLayout->addWidget(plantLabel);
    plantLayout->addWidget(sunflowerButton);
    plantLayout->addWidget(peashooterButton);
    plantLayout->addStretch();
    mainLayout->addLayout(plantLayout);
    
    // Game area - use QGraphicsView instead of QLabel
    gameView->setFixedSize(900, 500);
    gameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainLayout->addWidget(gameView, 1);
    
    // Control buttons
    QHBoxLayout* controlLayout = new QHBoxLayout();
    controlLayout->addWidget(startButton);
    controlLayout->addWidget(pauseButton);
    controlLayout->addStretch();
    controlLayout->addWidget(testPlantsButton);
    controlLayout->addWidget(testZombiesButton);
    mainLayout->addLayout(controlLayout);
    
    // Initial button states
    pauseButton->setEnabled(false);
    testPlantsButton->setEnabled(false);
    testZombiesButton->setEnabled(false);
    sunflowerButton->setEnabled(false);
    peashooterButton->setEnabled(false);
}

void MainWindow::onGameStarted()
{
    statusLabel->setText("Game Running");
    startButton->setEnabled(false);
    pauseButton->setEnabled(true);
    testPlantsButton->setEnabled(true);
    testZombiesButton->setEnabled(true);
    sunflowerButton->setEnabled(true);
    peashooterButton->setEnabled(true);
    updateStatusLabel();
}

void MainWindow::onGameStopped()
{
    statusLabel->setText("Game Stopped");
    startButton->setEnabled(true);
    pauseButton->setEnabled(false);
    testPlantsButton->setEnabled(false);
    testZombiesButton->setEnabled(false);
    sunflowerButton->setEnabled(false);
    peashooterButton->setEnabled(false);
    updateStatusLabel();
}

void MainWindow::onSunPointsChanged(int amount)
{
    sunPointsLabel->setText(QString("Sun: %1").arg(amount));
}

void MainWindow::onTickUpdate(int tickCount)
{
    tickLabel->setText(QString("Tick: %1").arg(tickCount));
}

void MainWindow::startButtonClicked()
{
    game->start();
}

void MainWindow::pauseButtonClicked()
{
    if (game->isPaused()) {
        game->resume();
        pauseButton->setText("Pause");
        statusLabel->setText("Game Running");
    } else {
        game->pause();
        pauseButton->setText("Resume");
        statusLabel->setText("Game Paused");
    }
}

void MainWindow::testPlacePlants()
{
    qDebug() << "testPlacePlants called";
    qDebug() << "Current sun points:" << game->getSunPoints();
    
    // Test: Place some plants on the grid
    // Place a Sunflower at position (2, 1)
    Sunflower* sunflower = new Sunflower();
    qDebug() << "Attempting to place Sunflower (cost:" << sunflower->getCost() << ") at (2, 1)";
    if (game->placePlant(sunflower, 2, 1)) {
        statusLabel->setText("Placed Sunflower at (2, 1)");
        qDebug() << "Successfully placed Sunflower";
    } else {
        delete sunflower;
        statusLabel->setText("Failed to place Sunflower");
        qDebug() << "Failed to place Sunflower";
    }
    
    // Place a Peashooter at position (2, 2)
    Peashooter* peashooter = new Peashooter();
    qDebug() << "Attempting to place Peashooter (cost:" << peashooter->getCost() << ") at (2, 2)";
    qDebug() << "Current sun points:" << game->getSunPoints();
    if (game->placePlant(peashooter, 2, 2)) {
        statusLabel->setText("Placed Peashooter at (2, 2)");
        qDebug() << "Successfully placed Peashooter";
    } else {
        delete peashooter;
        statusLabel->setText("Failed to place Peashooter");
        qDebug() << "Failed to place Peashooter";
    }
}

void MainWindow::testSpawnZombies()
{
    // Test: Spawn a zombie in row 2
    NormalZombie* zombie = new NormalZombie(2);
    game->spawnZombie(zombie);
    statusLabel->setText("Spawned Normal Zombie in row 2");
}

void MainWindow::updateStatusLabel()
{
    QString status = QString("Game: %1 | Grid: %2x%3")
                        .arg(game->isRunning() ? "Running" : "Stopped")
                        .arg(game->getGridRows())
                        .arg(game->getGridCols());
    statusLabel->setText(status);
}

void MainWindow::selectSunflower()
{
    gameScene->setPlantToPlace("Sunflower");
    statusLabel->setText("Click on grid to place Sunflower (Cost: 50)");
    qDebug() << "Sunflower selected for placement";
}

void MainWindow::selectPeashooter()
{
    gameScene->setPlantToPlace("Peashooter");
    statusLabel->setText("Click on grid to place Peashooter (Cost: 100)");
    qDebug() << "Peashooter selected for placement";
}
