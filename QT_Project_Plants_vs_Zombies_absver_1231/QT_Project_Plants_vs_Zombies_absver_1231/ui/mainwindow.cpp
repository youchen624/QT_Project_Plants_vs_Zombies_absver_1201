#include "mainwindow.h"
#include "Sunflower.h"
#include "Peashooter.h"
#include "NormalZombie.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , game(new Game(this))
    , statusLabel(new QLabel("Game Ready", this))
    , sunPointsLabel(new QLabel("Sun: 150", this))
    , tickLabel(new QLabel("Tick: 0", this))
    , startButton(new QPushButton("Start Game", this))
    , pauseButton(new QPushButton("Pause", this))
    , testPlantsButton(new QPushButton("Test: Place Plants", this))
    , testZombiesButton(new QPushButton("Test: Spawn Zombies", this))
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
    
    // Game area placeholder
    QLabel* gameAreaLabel = new QLabel("Game Area (5 rows x 9 columns grid)", this);
    gameAreaLabel->setStyleSheet("background-color: lightgreen; padding: 100px;");
    gameAreaLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(gameAreaLabel, 1);
    
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
}

void MainWindow::onGameStarted()
{
    statusLabel->setText("Game Running");
    startButton->setEnabled(false);
    pauseButton->setEnabled(true);
    testPlantsButton->setEnabled(true);
    testZombiesButton->setEnabled(true);
    updateStatusLabel();
}

void MainWindow::onGameStopped()
{
    statusLabel->setText("Game Stopped");
    startButton->setEnabled(true);
    pauseButton->setEnabled(false);
    testPlantsButton->setEnabled(false);
    testZombiesButton->setEnabled(false);
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
    // Test: Place some plants on the grid
    // Place a Sunflower at position (2, 1)
    Sunflower* sunflower = new Sunflower();
    if (game->placePlant(sunflower, 2, 1)) {
        statusLabel->setText("Placed Sunflower at (2, 1)");
    } else {
        delete sunflower;
        statusLabel->setText("Failed to place Sunflower");
    }
    
    // Place a Peashooter at position (2, 2)
    Peashooter* peashooter = new Peashooter();
    if (game->placePlant(peashooter, 2, 2)) {
        statusLabel->setText("Placed Peashooter at (2, 2)");
    } else {
        delete peashooter;
        statusLabel->setText("Failed to place Peashooter");
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
