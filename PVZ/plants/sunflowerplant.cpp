#include "sunflowerplant.h"
#include <QPainter>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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

// Twin Sunflower Plant Implementation
TwinSunflowerPlant::TwinSunflowerPlant(QObject *parent)
    : Plant(parent)
    , m_produceTimer(0.0f)
    , m_produceInterval(10.0f)
{
    m_health = 180;
    m_cost = 150;
    m_cooldown = 10.0f;
}

void TwinSunflowerPlant::update(float deltaTime)
{
    m_produceTimer += deltaTime;
    if (m_produceTimer >= m_produceInterval) {
        m_produceTimer = 0.0f;
        // Produces 3x more sun than regular sunflower (75 instead of 25)
        emit sunProduced(75);
    }
}

void TwinSunflowerPlant::render(QPainter *painter)
{
    if (!painter) return;
    
    // Draw two yellow flowers side by side
    painter->setBrush(Qt::yellow);
    painter->setPen(Qt::darkYellow);
    
    // Left flower
    painter->drawEllipse(m_position + QPoint(-12, 0), 20, 20);
    // Right flower
    painter->drawEllipse(m_position + QPoint(12, 0), 20, 20);
    
    // Draw petals for left flower
    painter->setBrush(QColor(255, 200, 0));
    for (int i = 0; i < 6; i++) {
        float angle = i * 60.0f * static_cast<float>(M_PI) / 180.0f;
        int x = m_position.x() - 12 + static_cast<int>(16 * cos(angle));
        int y = m_position.y() + static_cast<int>(16 * sin(angle));
        painter->drawEllipse(QPoint(x, y), 8, 8);
    }
    
    // Draw petals for right flower
    for (int i = 0; i < 6; i++) {
        float angle = i * 60.0f * static_cast<float>(M_PI) / 180.0f;
        int x = m_position.x() + 12 + static_cast<int>(16 * cos(angle));
        int y = m_position.y() + static_cast<int>(16 * sin(angle));
        painter->drawEllipse(QPoint(x, y), 8, 8);
    }
}
