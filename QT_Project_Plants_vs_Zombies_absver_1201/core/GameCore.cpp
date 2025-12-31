#include "GameCore.h"
#include "plant/Sunflower.h"
#include "plant/Peashooter.h"
#include "plant/Wallnut.h"
#include "zombie/General.h"
#include "zombie/Conehead.h"
#include "Bullet.h"
#include "Sun.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMouseEvent>
#include <QRandomGenerator>

using namespace PVZ::Core;

GameCore::GameCore() : state(GameState::Progressing), sun_count(150), 
                       tick_count(0), zombie_spawn_ticks(0)
{
    this->setWindowTitle(tr("Plants vs Zombies (abs-ver)"));
    m_scene = new QGraphicsScene(0, 0, 1400, 600);
    m_view  = new QGraphicsView(m_scene);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setFixedSize(1400, 600);
    
    // Initialize plants grid
    for (int i = 0; i < GRID_ROWS; i++) {
        for (int j = 0; j < GRID_COLS; j++) {
            plants[i][j] = nullptr;
        }
    }
    
    // Create UI container
    QWidget* container = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(container);
    
    // Sun counter
    QHBoxLayout* topBar = new QHBoxLayout();
    sun_label = new QLabel(QString("Sun: %1").arg(sun_count));
    sun_label->setStyleSheet("QLabel { font-size: 20px; font-weight: bold; color: yellow; background-color: rgba(0,0,0,100); padding: 5px; }");
    topBar->addWidget(sun_label);
    
    // Plant buttons
    QPushButton* btnSunflower = new QPushButton("Sunflower (50)");
    QPushButton* btnPeashooter = new QPushButton("Peashooter (100)");
    QPushButton* btnWallnut = new QPushButton("Wallnut (50)");
    
    connect(btnSunflower, &QPushButton::clicked, [this]() {
        if (sun_count >= 50) {
            plantAt("sunflower", 2, 1);
        }
    });
    connect(btnPeashooter, &QPushButton::clicked, [this]() {
        if (sun_count >= 100) {
            plantAt("peashooter", 2, 2);
        }
    });
    connect(btnWallnut, &QPushButton::clicked, [this]() {
        if (sun_count >= 50) {
            plantAt("wallnut", 2, 0);
        }
    });
    
    topBar->addWidget(btnSunflower);
    topBar->addWidget(btnPeashooter);
    topBar->addWidget(btnWallnut);
    topBar->addStretch();
    
    layout->addLayout(topBar);
    layout->addWidget(m_view);
    layout->setContentsMargins(0, 0, 0, 0);
    
    setCentralWidget(container);
    setFixedSize(1400, 640);

    // Add background
    Ground* ground = new Ground(this);

    // Draw grid lines for debugging
    QPen gridPen(QColor(100, 100, 100, 100));
    for (int i = 0; i <= GRID_ROWS; i++) {
        m_scene->addLine(GRID_START_X, GRID_START_Y + i * GRID_SIZE,
                        GRID_START_X + GRID_COLS * GRID_SIZE, GRID_START_Y + i * GRID_SIZE,
                        gridPen);
    }
    for (int j = 0; j <= GRID_COLS; j++) {
        m_scene->addLine(GRID_START_X + j * GRID_SIZE, GRID_START_Y,
                        GRID_START_X + j * GRID_SIZE, GRID_START_Y + GRID_ROWS * GRID_SIZE,
                        gridPen);
    }

    // Connect timer to game tick
    connect(&m_timer, &QTimer::timeout, this, &GameCore::gameTick);
    
    // Add some initial plants for testing
    plantAt("sunflower", 0, 2);
    plantAt("sunflower", 1, 1);
    plantAt("peashooter", 2, 3);
    plantAt("wallnut", 3, 0);
    
    // Start the game
    start();
}

GameCore::~GameCore() {
    // Cleanup
    for (int i = 0; i < GRID_ROWS; i++) {
        for (int j = 0; j < GRID_COLS; j++) {
            if (plants[i][j]) delete plants[i][j];
        }
    }
    qDeleteAll(zombies);
    qDeleteAll(bullets);
    qDeleteAll(suns);
}

void GameCore::gameTick() {
    emit coreTick();
    tick_count++;
    
    // Spawn zombies periodically
    zombie_spawn_ticks++;
    if (zombie_spawn_ticks >= 200) {  // Spawn every 10 seconds
        spawnZombieWave();
        zombie_spawn_ticks = 0;
    }
    
    // Spawn sun from sky occasionally
    if (tick_count % 300 == 0) {
        qreal x = QRandomGenerator::global()->bounded(800) + 300;
        spawnSun(x, 0, true);
    }
    
    // Check collisions
    checkCollisions();
    
    // Cleanup dead objects
    cleanupDeadObjects();
}

void GameCore::plantAt(QString type, int row, int col) {
    if (row < 0 || row >= GRID_ROWS || col < 0 || col >= GRID_COLS) return;
    if (plants[row][col] != nullptr) return;  // Already a plant there
    
    Plant::Base* plant = nullptr;
    int cost = 0;
    
    if (type == "sunflower") {
        cost = 50;
        if (sun_count >= cost) {
            plant = new Plant::Sunflower(this, row, col);
        }
    } else if (type == "peashooter") {
        cost = 100;
        if (sun_count >= cost) {
            plant = new Plant::Peashooter(this, row, col);
        }
    } else if (type == "wallnut") {
        cost = 50;
        if (sun_count >= cost) {
            plant = new Plant::Wallnut(this, row, col);
        }
    }
    
    if (plant) {
        plants[row][col] = plant;
        removeSun(cost);
        qDebug() << "Planted" << type << "at" << row << "," << col;
    }
}

void GameCore::spawnZombie(QString type, int row) {
    if (row < 0 || row >= GRID_ROWS) return;
    
    Zombie::Base* zombie = nullptr;
    
    if (type == "general") {
        zombie = new Zombie::General(this, row);
    } else if (type == "conehead") {
        zombie = new Zombie::Conehead(this, row);
    }
    
    if (zombie) {
        zombies.append(zombie);
        qDebug() << "Spawned" << type << "zombie at row" << row;
    }
}

void GameCore::spawnSun(qreal x, qreal y, bool fromSky) {
    Sun* sun = new Sun(this, x, y, fromSky);
    suns.append(sun);
}

void GameCore::collectSun(Sun* sun) {
    if (sun && sun->isCollectable()) {
        addSun(sun->getValue());
        sun->collect();
    }
}

void GameCore::updateSunDisplay() {
    if (sun_label) {
        sun_label->setText(QString("Sun: %1").arg(sun_count));
    }
}

void GameCore::checkCollisions() {
    // Check bullet-zombie collisions
    for (int i = bullets.size() - 1; i >= 0; i--) {
        Bullet* bullet = bullets[i];
        if (!bullet->isActive()) continue;
        
        for (Zombie::Base* zombie : zombies) {
            if (zombie->isDead()) continue;
            if (zombie->getRow() != bullet->getRow()) continue;
            
            // Simple collision detection using defined thresholds
            qreal dx = zombie->getX() - bullet->getX();
            if (dx < BULLET_COLLISION_THRESHOLD_FRONT && dx > BULLET_COLLISION_THRESHOLD_BACK) {
                zombie->takeDamage(bullet->getDamage());
                bullet->deactivate();
                break;
            }
        }
    }
    
    // Check zombie-plant collisions
    for (Zombie::Base* zombie : zombies) {
        if (zombie->isDead()) continue;
        
        int row = zombie->getRow();
        qreal zx = zombie->getX();
        
        // Check which grid column the zombie is in
        int col = (zx - GRID_START_X) / GRID_SIZE;
        
        if (col >= 0 && col < GRID_COLS && row >= 0 && row < GRID_ROWS) {
            Plant::Base* plant = plants[row][col];
            if (plant && !plant->isDead()) {
                // Zombie should start eating
                if (!zombie->isEating()) {
                    zombie->setEating(true);
                }
                
                // Plant takes damage when zombie attacks
                if (zombie->isEating()) {
                    plant->takeDamage(1);  // 1 damage per tick while eating
                    if (plant->isDead()) {
                        delete plant;
                        plants[row][col] = nullptr;
                        zombie->setEating(false);
                    }
                }
            } else {
                zombie->setEating(false);
            }
        } else {
            zombie->setEating(false);
        }
        
        // Check if zombie reached the house
        if (zx < 100) {
            state = GameState::GameOver_lose;
            pause();
            qDebug() << "Game Over! Zombies reached your house!";
        }
    }
}

void GameCore::cleanupDeadObjects() {
    // Remove dead zombies
    for (int i = zombies.size() - 1; i >= 0; i--) {
        if (zombies[i]->isDead()) {
            delete zombies[i];
            zombies.removeAt(i);
        }
    }
    
    // Remove inactive bullets
    for (int i = bullets.size() - 1; i >= 0; i--) {
        if (!bullets[i]->isActive()) {
            delete bullets[i];
            bullets.removeAt(i);
        }
    }
    
    // Remove collected/expired suns
    for (int i = suns.size() - 1; i >= 0; i--) {
        Sun* sun = suns[i];
        if (sun->isCollected()) {
            delete sun;
            suns.removeAt(i);
        }
    }
}

void GameCore::spawnZombieWave() {
    int numZombies = 1 + (tick_count / 2000);  // Increase difficulty over time
    if (numZombies > 5) numZombies = 5;
    
    for (int i = 0; i < numZombies; i++) {
        int row = QRandomGenerator::global()->bounded(GRID_ROWS);
        QString type = (QRandomGenerator::global()->bounded(3) == 0) ? "conehead" : "general";
        spawnZombie(type, row);
    }
}

QPixmap Ground::pix = QPixmap(":/.resources/img/background/background.png");

Ground::Ground(GameCore* core) : core(core) {
    graphic_main.setPixmap(pix);
    core->m_scene->addItem(&graphic_main);
    graphic_main.setPos(0, 0);
}

Ground::~Ground() {
}

void GameCore::mousePressEvent(QMouseEvent* event) {
    // Convert window coordinates to scene coordinates
    QPoint viewPos = m_view->mapFromParent(event->pos());
    QPointF scenePos = m_view->mapToScene(viewPos);
    
    // Check if clicked on a sun
    for (Sun* sun : suns) {
        if (sun->isCollectable() && sun->contains(scenePos.x(), scenePos.y())) {
            collectSun(sun);
            break;
        }
    }
    
    QMainWindow::mousePressEvent(event);
}
