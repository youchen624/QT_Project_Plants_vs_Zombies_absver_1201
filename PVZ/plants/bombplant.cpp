#include "bombplant.h"
#include <QPainter>
#include <QRadialGradient>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static const int INFINITE_HEALTH = 999999;
static const float NUCLEAR_BOMB_COOLDOWN = 45.0f;
static const float DEGREES_TO_RADIANS = static_cast<float>(M_PI) / 180.0f;
static const float FLAME_ANGLE_DEGREES = 120.0f;

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

// Super Nuclear Bomb Plant Implementation
SuperNuclearBombPlant::SuperNuclearBombPlant(QObject *parent)
    : Plant(parent)
    , m_detonateTimer(0.0f)
    , m_detonateDelay(5.0f)
    , m_hasExploded(false)
    , m_animationTimer(0.0f)
{
    m_health = INFINITE_HEALTH;
    m_cost = 1500;
    m_cooldown = 120.0f; // Very long cooldown
}

void SuperNuclearBombPlant::update(float deltaTime)
{
    if (!m_hasExploded) {
        m_detonateTimer += deltaTime;
        if (m_detonateTimer >= m_detonateDelay) {
            m_hasExploded = true;
            emit superExplode();
            // Don't delete immediately - let animation play
        }
    } else {
        m_animationTimer += deltaTime;
        // Delete after animation completes (2 seconds)
        if (m_animationTimer >= 2.0f) {
            markForDeletion();
        }
    }
}

void SuperNuclearBombPlant::render(QPainter *painter)
{
    if (!painter) return;
    
    if (!m_hasExploded) {
        // Calculate pulse effect based on timer
        float pulsePhase = m_detonateTimer / m_detonateDelay;
        int pulseSize = static_cast<int>(35 + 15 * pulsePhase);
        
        // Draw a massive red/orange nuclear bomb that pulses
        QColor bombColor;
        if (pulsePhase < 0.3f) {
            bombColor = QColor(255, 0, 0); // Bright red
        } else if (pulsePhase < 0.6f) {
            bombColor = QColor(255, 140, 0); // Orange
        } else {
            // Flash between bright colors as it's about to explode
            bombColor = (static_cast<int>(pulsePhase * 30) % 2 == 0) ? 
                        QColor(255, 255, 0) : QColor(255, 0, 0);
        }
        
        painter->setBrush(bombColor);
        painter->setPen(QPen(Qt::darkRed, 3));
        painter->drawEllipse(m_position, pulseSize, pulseSize);
        
        // Draw larger nuclear symbol
        painter->setPen(QPen(Qt::black, 3));
        painter->setBrush(Qt::yellow);
        
        // Center circle
        painter->drawEllipse(m_position, 12, 12);
        
        // Three radiation triangles
        for (int i = 0; i < 3; i++) {
            float angle = i * 120.0f * static_cast<float>(M_PI) / 180.0f;
            int x1 = m_position.x() + static_cast<int>(22 * cos(angle));
            int y1 = m_position.y() + static_cast<int>(22 * sin(angle));
            
            QPolygon triangle;
            triangle << QPoint(m_position.x(), m_position.y())
                     << QPoint(x1 + static_cast<int>(8 * cos(angle + M_PI/2)), 
                              y1 + static_cast<int>(8 * sin(angle + M_PI/2)))
                     << QPoint(x1 + static_cast<int>(8 * cos(angle - M_PI/2)), 
                              y1 + static_cast<int>(8 * sin(angle - M_PI/2)));
            painter->drawPolygon(triangle);
        }
        
        // Draw countdown timer
        painter->setFont(QFont("Arial", 14, QFont::Bold));
        painter->setPen(Qt::white);
        int countdown = static_cast<int>(m_detonateDelay - m_detonateTimer) + 1;
        if (countdown > 0) {
            painter->drawText(m_position.x() - 8, m_position.y() + 50, QString::number(countdown));
        }
        
        // Draw "SUPER" text
        painter->setFont(QFont("Arial", 8, QFont::Bold));
        painter->setPen(Qt::red);
        painter->drawText(m_position.x() - 15, m_position.y() - 45, "SUPER");
    } else {
        // Explosion animation
        float animPhase = m_animationTimer / 2.0f;
        int radius = static_cast<int>(50 + animPhase * 400); // Huge explosion
        
        // Multiple layers of explosion
        QRadialGradient gradient(m_position, radius);
        gradient.setColorAt(0, QColor(255, 255, 255, 255 - static_cast<int>(animPhase * 255)));
        gradient.setColorAt(0.3, QColor(255, 200, 0, 200 - static_cast<int>(animPhase * 200)));
        gradient.setColorAt(0.6, QColor(255, 100, 0, 150 - static_cast<int>(animPhase * 150)));
        gradient.setColorAt(1, QColor(255, 0, 0, 50 - static_cast<int>(animPhase * 50)));
        
        painter->setBrush(gradient);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(m_position, radius, radius);
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
