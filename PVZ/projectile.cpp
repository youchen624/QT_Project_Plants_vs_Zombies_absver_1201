#include "projectile.h"
#include <QPainter>
#include <cmath>

static const int PROJECTILE_HEALTH = 1;

Projectile::Projectile(int damage, float speed, ProjectileType type, QObject *parent)
    : Entity(parent)
    , m_damage(damage)
    , m_speed(speed)
    , m_type(type)
    , m_target(nullptr)
    , m_exactX(0.0f)
    , m_exactY(0.0f)
{
    m_health = PROJECTILE_HEALTH; // Projectiles are destroyed on hit
}

Projectile::~Projectile()
{
}

void Projectile::update(float deltaTime)
{
    // Initialize exact positions if not set
    if (m_exactX == 0.0f && m_exactY == 0.0f) {
        m_exactX = static_cast<float>(m_position.x());
        m_exactY = static_cast<float>(m_position.y());
    }
    
    if (m_type == TRACKING && m_target && m_target->isAlive()) {
        // Track the target
        QPoint targetPos = m_target->getPosition();
        
        // Calculate direction to target
        float dx = targetPos.x() - m_exactX;
        float dy = targetPos.y() - m_exactY;
        float distance = std::sqrt(dx * dx + dy * dy);
        
        if (distance > 0.1f) {
            // Normalize and move towards target
            dx /= distance;
            dy /= distance;
            
            m_exactX += dx * m_speed * deltaTime;
            m_exactY += dy * m_speed * deltaTime;
            
            m_position.setX(static_cast<int>(m_exactX));
            m_position.setY(static_cast<int>(m_exactY));
        }
    } else {
        // Move right (towards zombies) for normal projectiles
        m_exactX += m_speed * deltaTime;
        m_position.setX(static_cast<int>(m_exactX));
    }
}

void Projectile::render(QPainter *painter)
{
    if (!painter) return;
    
    if (m_type == ICE) {
        // Draw a blue/cyan ice projectile
        painter->setBrush(QColor(100, 200, 255));
        painter->setPen(QColor(50, 150, 255));
        painter->drawEllipse(m_position, 8, 8);
        // Add ice crystal effect
        painter->setBrush(QColor(200, 230, 255));
        painter->drawEllipse(m_position, 4, 4);
    } else if (m_type == TRACKING) {
        // Draw a purple/pink tracking projectile
        painter->setBrush(QColor(200, 100, 200));
        painter->setPen(QColor(150, 50, 150));
        painter->drawEllipse(m_position, 10, 10);
        // Add glowing effect
        painter->setBrush(QColor(255, 150, 255));
        painter->drawEllipse(m_position, 5, 5);
    } else {
        // Draw a green pea projectile
        painter->setBrush(QColor(0, 200, 0));
        painter->setPen(Qt::darkGreen);
        painter->drawEllipse(m_position, 8, 8);
    }
}
