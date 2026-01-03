#include "zombiebase.h"
#include <QPainter>

Zombie::Zombie(QObject *parent)
    : Entity(parent)
    , m_speed(15.0f)
    , m_attackDamage(20)
    , m_attackInterval(1.5f)
    , m_attackTimer(0.0f)
    , m_isAttacking(false)
    , m_exactX(0.0f)
    , m_slowTimer(0.0f)
    , m_slowMultiplier(0.5f)
{
    m_health = 200;
}

Zombie::~Zombie()
{
}

void Zombie::applySlow(float duration)
{
    m_slowTimer = duration;
}

float Zombie::getEffectiveSpeed() const
{
    if (m_slowTimer > 0.0f) {
        return m_speed * m_slowMultiplier;
    }
    return m_speed;
}

// Skeleton Implementation
Skeleton::Skeleton(QObject *parent)
    : Entity(parent)
    , m_speed(60.0f)
    , m_direction(1)
    , m_attackDamage(15) // Reduced from 30
    , m_attackInterval(1.5f)
    , m_attackTimer(0.0f)
    , m_isAttacking(false)
    , m_exactX(0.0f)
{
    m_health = 50; // Reduced from 100
}

void Skeleton::update(float deltaTime)
{
    if (!m_isAttacking) {
        // Move in the direction (right towards zombies)
        m_exactX += m_direction * m_speed * deltaTime;
        m_position.setX(static_cast<int>(m_exactX));
    } else {
        // Decrease attack timer when attacking
        if (m_attackTimer > 0.0f) {
            m_attackTimer -= deltaTime;
        }
    }
}

void Skeleton::render(QPainter *painter)
{
    if (!painter) return;
    
    // Draw skeleton - white bones
    painter->setBrush(Qt::white);
    painter->setPen(Qt::black);
    
    // Head (skull)
    painter->drawEllipse(m_position + QPoint(0, -20), 12, 12);
    
    // Eye sockets
    painter->setBrush(Qt::black);
    painter->drawEllipse(m_position + QPoint(-5, -22), 3, 4);
    painter->drawEllipse(m_position + QPoint(5, -22), 3, 4);
    
    // Body (spine/ribs)
    painter->setBrush(Qt::white);
    painter->drawRect(m_position.x() - 3, m_position.y() - 8, 6, 15);
    
    // Draw ribs
    painter->setPen(QPen(Qt::black, 1));
    for (int i = 0; i < 4; i++) {
        int y = m_position.y() - 6 + i * 4;
        painter->drawLine(m_position.x() - 3, y, m_position.x() - 10, y);
        painter->drawLine(m_position.x() + 3, y, m_position.x() + 10, y);
    }
    
    // Arms
    painter->setPen(QPen(Qt::white, 3));
    painter->drawLine(m_position.x() - 5, m_position.y() - 5, m_position.x() - 12, m_position.y() + 5);
    painter->drawLine(m_position.x() + 5, m_position.y() - 5, m_position.x() + 12, m_position.y() + 5);
    
    // Legs
    painter->drawLine(m_position.x() - 3, m_position.y() + 7, m_position.x() - 8, m_position.y() + 20);
    painter->drawLine(m_position.x() + 3, m_position.y() + 7, m_position.x() + 8, m_position.y() + 20);
}
