#include "plant.h"
#include <QPainter>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Rendering constants
static const float SNOWFLAKE_ANGLE_INCREMENT = 90.0f;  // Degrees between snowflake arms
static const float FLAME_ANGLE_DEGREES = 120.0f;       // Degrees between flame elements
static const float DEGREES_TO_RADIANS = static_cast<float>(M_PI) / 180.0f; // Conversion factor
static const int INFINITE_HEALTH = 999999;             // Effectively infinite health for plants that should be ignored by zombies

// Plant timing constants
static const float NUCLEAR_BOMB_COOLDOWN = 45.0f;      // Nuclear bomb cooldown in seconds

Plant::Plant(QObject *parent)
    : Entity(parent)
    , m_cost(50)
    , m_cooldown(5.0f)
{
}

Plant::~Plant()
{
}

// Shooter Plant Implementation
ShooterPlant::ShooterPlant(QObject *parent)
    : Plant(parent)
    , m_shootTimer(0.0f)
    , m_shootInterval(2.0f)
    , m_damage(20)
{
    m_health = 200;
    m_cost = 100;
}

void ShooterPlant::update(float deltaTime)
{
    m_shootTimer += deltaTime;
    // Shooting logic will be handled by game scene
}

void ShooterPlant::render(QPainter *painter)
{
    if (!painter) return;
    
    // Draw a simple green circle for shooter
    painter->setBrush(Qt::green);
    painter->setPen(Qt::darkGreen);
    painter->drawEllipse(m_position, 25, 25);
    
    // Draw shooter indicator
    painter->setBrush(Qt::yellow);
    painter->drawEllipse(m_position + QPoint(15, 0), 8, 8);
}

// Wall Plant Implementation
WallPlant::WallPlant(QObject *parent)
    : Plant(parent)
{
    m_health = 600;
    m_cost = 50;
}

void WallPlant::update(float /*deltaTime*/)
{
    // Wall plants are passive
}

void WallPlant::render(QPainter *painter)
{
    if (!painter) return;
    
    // Draw a brown square for wall
    painter->setBrush(QColor(139, 69, 19));
    painter->setPen(Qt::black);
    painter->drawRect(m_position.x() - 20, m_position.y() - 20, 40, 40);
}

// Sunflower Plant Implementation
SunflowerPlant::SunflowerPlant(QObject *parent)
    : Plant(parent)
    , m_produceTimer(0.0f)
    , m_produceInterval(10.0f)
{
    m_health = 150;
    m_cost = 50;
}

void SunflowerPlant::update(float deltaTime)
{
    m_produceTimer += deltaTime;
    if (m_produceTimer >= m_produceInterval) {
        m_produceTimer = 0.0f;
        emit sunProduced(25);
    }
}

void SunflowerPlant::render(QPainter *painter)
{
    if (!painter) return;
    
    // Draw a yellow flower for sunflower
    painter->setBrush(Qt::yellow);
    painter->setPen(Qt::darkYellow);
    painter->drawEllipse(m_position, 25, 25);
    
    // Draw petals
    painter->setBrush(QColor(255, 200, 0));
    for (int i = 0; i < 6; i++) {
        float angle = i * 60.0f * static_cast<float>(M_PI) / 180.0f;
        int x = m_position.x() + static_cast<int>(20 * cos(angle));
        int y = m_position.y() + static_cast<int>(20 * sin(angle));
        painter->drawEllipse(QPoint(x, y), 10, 10);
    }
}

// Ice Shooter Plant Implementation
IceShooterPlant::IceShooterPlant(QObject *parent)
    : Plant(parent)
    , m_shootTimer(0.0f)
    , m_shootInterval(2.5f)
    , m_damage(15)
{
    m_health = 200;
    m_cost = 175;
}

void IceShooterPlant::update(float deltaTime)
{
    m_shootTimer += deltaTime;
    // Shooting logic will be handled by game scene
}

void IceShooterPlant::render(QPainter *painter)
{
    if (!painter) return;
    
    // Draw a cyan/ice blue circle for ice shooter
    painter->setBrush(QColor(100, 200, 255));
    painter->setPen(QColor(50, 150, 255));
    painter->drawEllipse(m_position, 25, 25);
    
    // Draw ice crystal indicator
    painter->setBrush(QColor(200, 230, 255));
    painter->setPen(QColor(150, 200, 255));
    // Draw snowflake pattern
    for (int i = 0; i < 4; i++) {
        float angle = i * SNOWFLAKE_ANGLE_INCREMENT * static_cast<float>(M_PI) / 180.0f;
        int x1 = m_position.x() + static_cast<int>(15 * cos(angle));
        int y1 = m_position.y() + static_cast<int>(15 * sin(angle));
        int x2 = m_position.x() + static_cast<int>(8 * cos(angle));
        int y2 = m_position.y() + static_cast<int>(8 * sin(angle));
        painter->drawLine(x1, y1, x2, y2);
    }
}

// Repeater Plant Implementation
RepeaterPlant::RepeaterPlant(QObject *parent)
    : Plant(parent)
    , m_shootTimer(0.0f)
    , m_shootInterval(1.8f)
    , m_damage(40)
{
    m_health = 200;
    m_cost = 200;
}

void RepeaterPlant::update(float deltaTime)
{
    m_shootTimer += deltaTime;
    // Shooting logic will be handled by game scene
}

void RepeaterPlant::render(QPainter *painter)
{
    if (!painter) return;
    
    // Draw a darker green circle for repeater
    painter->setBrush(QColor(0, 150, 0));
    painter->setPen(QColor(0, 100, 0));
    painter->drawEllipse(m_position, 25, 25);
    
    // Draw two shooter indicators for double shot
    painter->setBrush(Qt::yellow);
    painter->drawEllipse(m_position + QPoint(12, -5), 7, 7);
    painter->drawEllipse(m_position + QPoint(12, 5), 7, 7);
}

// Nuclear Bomb Plant Implementation
NuclearBombPlant::NuclearBombPlant(QObject *parent)
    : Plant(parent)
    , m_detonateTimer(0.0f)
    , m_detonateDelay(3.0f)
    , m_hasExploded(false)
{
    m_health = INFINITE_HEALTH; // Effectively infinite health - zombies ignore it
    m_cost = 150;
    m_cooldown = NUCLEAR_BOMB_COOLDOWN; // Much longer cooldown
}

void NuclearBombPlant::update(float deltaTime)
{
    if (!m_hasExploded) {
        m_detonateTimer += deltaTime;
        if (m_detonateTimer >= m_detonateDelay) {
            m_hasExploded = true;
            emit explode();
            // Mark for deletion after a brief moment to ensure signal is processed
            // The actual deletion happens in the next game loop iteration
            markForDeletion();
        }
    }
}

void NuclearBombPlant::render(QPainter *painter)
{
    if (!painter) return;
    
    // Calculate pulse effect based on timer
    float pulsePhase = m_detonateTimer / m_detonateDelay;
    int pulseSize = static_cast<int>(25 + 10 * pulsePhase);
    
    // Draw a red/orange nuclear bomb that pulses
    QColor bombColor;
    if (pulsePhase < 0.5f) {
        bombColor = QColor(255, 69, 0); // Orange-red
    } else {
        // Flash red as it's about to explode
        bombColor = (static_cast<int>(pulsePhase * 20) % 2 == 0) ? Qt::red : QColor(255, 140, 0);
    }
    
    painter->setBrush(bombColor);
    painter->setPen(Qt::darkRed);
    painter->drawEllipse(m_position, pulseSize, pulseSize);
    
    // Draw nuclear symbol
    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(Qt::yellow);
    
    // Center circle
    painter->drawEllipse(m_position, 8, 8);
    
    // Three radiation triangles
    for (int i = 0; i < 3; i++) {
        float angle = i * 120.0f * static_cast<float>(M_PI) / 180.0f;
        int x1 = m_position.x() + static_cast<int>(15 * cos(angle));
        int y1 = m_position.y() + static_cast<int>(15 * sin(angle));
        
        QPolygon triangle;
        triangle << QPoint(m_position.x(), m_position.y())
                 << QPoint(x1 + static_cast<int>(5 * cos(angle + M_PI/2)), 
                          y1 + static_cast<int>(5 * sin(angle + M_PI/2)))
                 << QPoint(x1 + static_cast<int>(5 * cos(angle - M_PI/2)), 
                          y1 + static_cast<int>(5 * sin(angle - M_PI/2)));
        painter->drawPolygon(triangle);
    }
    
    // Draw countdown timer
    painter->setFont(QFont("Arial", 10, QFont::Bold));
    painter->setPen(Qt::white);
    int countdown = static_cast<int>(m_detonateDelay - m_detonateTimer) + 1;
    if (countdown > 0) {
        painter->drawText(m_position.x() - 5, m_position.y() + 40, QString::number(countdown));
    }
}

// Skeleton Tree Plant Implementation
SkeletonTreePlant::SkeletonTreePlant(QObject *parent)
    : Plant(parent)
    , m_summonTimer(0.0f)
    , m_summonInterval(8.0f) // Summon skeleton every 8 seconds
{
    m_health = 150; // Reduced from 300 - more balanced
    m_cost = 350; // Increased from 175
    m_cooldown = 10.0f;
}

void SkeletonTreePlant::update(float deltaTime)
{
    m_summonTimer += deltaTime;
    if (m_summonTimer >= m_summonInterval) {
        m_summonTimer = 0.0f;
        // Emit signal to summon skeletons on adjacent lanes
        // The game scene will handle the actual skeleton creation
        emit summonSkeleton(m_row); // Pass tree's row for adjacent lane calculation
    }
}

void SkeletonTreePlant::render(QPainter *painter)
{
    if (!painter) return;
    
    // Draw a dark tree with skull
    painter->setBrush(QColor(80, 50, 30)); // Dark brown trunk
    painter->setPen(Qt::black);
    painter->drawRect(m_position.x() - 8, m_position.y() - 15, 16, 30);
    
    // Draw skull-like crown
    painter->setBrush(QColor(220, 220, 220)); // Light gray for skull
    painter->drawEllipse(m_position + QPoint(0, -15), 20, 18);
    
    // Draw skull eyes
    painter->setBrush(Qt::black);
    painter->drawEllipse(m_position + QPoint(-8, -18), 5, 7);
    painter->drawEllipse(m_position + QPoint(8, -18), 5, 7);
    
    // Draw skull nose
    QPolygon nose;
    nose << QPoint(m_position.x(), m_position.y() - 12)
         << QPoint(m_position.x() - 3, m_position.y() - 8)
         << QPoint(m_position.x() + 3, m_position.y() - 8);
    painter->drawPolygon(nose);
}

// Ground Spike Plant Implementation
GroundSpikePlant::GroundSpikePlant(QObject *parent)
    : Plant(parent)
    , m_damage(1) // Nerfed: Reduced from 2 to 1
{
    m_health = INFINITE_HEALTH; // Effectively infinite - ignored by zombies
    m_cost = 100;
    m_cooldown = 7.0f;
}

void GroundSpikePlant::update(float /*deltaTime*/)
{
    // Damage is applied in game scene collision detection
}

void GroundSpikePlant::render(QPainter *painter)
{
    if (!painter) return;
    
    // Draw ground spike as spiky base
    painter->setBrush(QColor(100, 100, 100)); // Gray spike base
    painter->setPen(Qt::darkGray);
    
    // Draw circular base
    painter->drawEllipse(m_position, 22, 12);
    
    // Draw multiple spikes pointing up
    painter->setBrush(QColor(150, 150, 150));
    for (int i = -2; i <= 2; i++) {
        QPolygon spike;
        int xOffset = i * 10;
        spike << QPoint(m_position.x() + xOffset - 4, m_position.y())
              << QPoint(m_position.x() + xOffset + 4, m_position.y())
              << QPoint(m_position.x() + xOffset, m_position.y() - 15);
        painter->drawPolygon(spike);
    }
}

// Chili Pepper Plant Implementation
ChiliPepperPlant::ChiliPepperPlant(QObject *parent)
    : Plant(parent)
    , m_activateTimer(0.0f)
    , m_activateDelay(2.0f) // Activate after 2 seconds
    , m_hasActivated(false)
{
    m_health = INFINITE_HEALTH; // Infinite health - can't be eaten
    m_cost = 125;
    m_cooldown = 25.0f;
}

void ChiliPepperPlant::update(float deltaTime)
{
    if (!m_hasActivated) {
        m_activateTimer += deltaTime;
        if (m_activateTimer >= m_activateDelay) {
            m_hasActivated = true;
            emit burnRow(m_row);
            // Mark for deletion after burning
            markForDeletion();
        }
    }
}

void ChiliPepperPlant::render(QPainter *painter)
{
    if (!painter) return;
    
    // Calculate intensity based on timer
    float intensity = m_activateTimer / m_activateDelay;
    
    // Draw a red chili pepper that glows brighter as it's about to activate
    QColor chiliColor;
    if (intensity < 0.5f) {
        chiliColor = QColor(200, 0, 0); // Dark red
    } else {
        // Flash bright red/orange as it's about to activate
        chiliColor = (static_cast<int>(intensity * 20) % 2 == 0) ? Qt::red : QColor(255, 100, 0);
    }
    
    painter->setBrush(chiliColor);
    painter->setPen(Qt::darkRed);
    
    // Draw chili pepper body (elongated oval)
    painter->drawEllipse(m_position, 12, 25);
    
    // Draw stem
    painter->setBrush(Qt::darkGreen);
    painter->drawRect(m_position.x() - 3, m_position.y() - 30, 6, 8);
    
    // Draw flame effect if close to activation
    if (intensity > 0.6f) {
        painter->setBrush(QColor(255, 200, 0, 180)); // Semi-transparent flame
        painter->setPen(Qt::NoPen);
        for (int i = 0; i < 3; i++) {
            float angle = i * FLAME_ANGLE_DEGREES * DEGREES_TO_RADIANS;
            int x = m_position.x() + static_cast<int>(20 * cos(angle));
            int y = m_position.y() + static_cast<int>(20 * sin(angle));
            painter->drawEllipse(QPoint(x, y), 8, 12);
        }
    }
}

// Mine Plant Implementation
MinePlant::MinePlant(QObject *parent)
    : Plant(parent)
    , m_shouldExplode(false)
    , m_explodeTimer(0.0f)
    , m_explodeDelay(0.3f) // Short delay before explosion
{
    m_health = 1; // Fragile, but zombies walk over it
    m_cost = 25;
    m_cooldown = 7.5f;
}

void MinePlant::update(float deltaTime)
{
    if (m_shouldExplode) {
        m_explodeTimer += deltaTime;
        if (m_explodeTimer >= m_explodeDelay) {
            emit explode(m_row, m_col);
            markForDeletion();
        }
    }
}

void MinePlant::render(QPainter *painter)
{
    if (!painter) return;
    
    // Draw mine - dark circle with spikes
    if (!m_shouldExplode) {
        // Mine body - dark gray circle
        painter->setBrush(QColor(60, 60, 60));
        painter->setPen(QPen(Qt::black, 2));
        painter->drawEllipse(m_position, 15, 15);
        
        // Draw spikes around the mine
        painter->setPen(QPen(QColor(80, 80, 80), 3));
        for (int i = 0; i < 8; i++) {
            float angle = i * 45.0f * DEGREES_TO_RADIANS;
            int x1 = m_position.x() + static_cast<int>(12 * cos(angle));
            int y1 = m_position.y() + static_cast<int>(12 * sin(angle));
            int x2 = m_position.x() + static_cast<int>(18 * cos(angle));
            int y2 = m_position.y() + static_cast<int>(18 * sin(angle));
            painter->drawLine(x1, y1, x2, y2);
        }
        
        // Warning symbol
        painter->setBrush(Qt::yellow);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(m_position, 5, 5);
    } else {
        // Explosion effect
        float intensity = m_explodeTimer / m_explodeDelay;
        int radius = static_cast<int>(20 + intensity * 30);
        
        // Orange/red explosion
        QRadialGradient gradient(m_position, radius);
        gradient.setColorAt(0, QColor(255, 200, 0, 200));
        gradient.setColorAt(0.5, QColor(255, 100, 0, 150));
        gradient.setColorAt(1, QColor(255, 0, 0, 50));
        
        painter->setBrush(gradient);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(m_position, radius, radius);
    }
}
