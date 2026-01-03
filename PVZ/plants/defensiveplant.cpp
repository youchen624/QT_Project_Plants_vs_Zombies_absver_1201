#include "defensiveplant.h"
#include <QPainter>

static const int INFINITE_HEALTH = 999999;

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
