#include "zombie.h"
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

// Normal Zombie Implementation
NormalZombie::NormalZombie(QObject *parent)
    : Zombie(parent)
{
    m_health = 200;
    m_speed = 15.0f;
    m_attackDamage = 20;
}

void NormalZombie::update(float deltaTime)
{
    // Update slow timer
    if (m_slowTimer > 0.0f) {
        m_slowTimer -= deltaTime;
        if (m_slowTimer < 0.0f) m_slowTimer = 0.0f;
    }
    
    if (!m_isAttacking) {
        // Move left using float precision with effective speed
        m_exactX -= getEffectiveSpeed() * deltaTime;
        m_position.setX(static_cast<int>(m_exactX));
    } else {
        // Decrease attack timer when attacking
        if (m_attackTimer > 0.0f) {
            m_attackTimer -= deltaTime;
        }
    }
}

void NormalZombie::render(QPainter *painter)
{
    if (!painter) return;
    
    // Draw a gray zombie
    painter->setBrush(QColor(128, 128, 128));
    painter->setPen(Qt::black);
    painter->drawEllipse(m_position, 20, 25);
    
    // Draw eyes
    painter->setBrush(Qt::red);
    painter->drawEllipse(m_position + QPoint(-8, -5), 4, 4);
    painter->drawEllipse(m_position + QPoint(8, -5), 4, 4);
}

// Fast Zombie Implementation
FastZombie::FastZombie(QObject *parent)
    : Zombie(parent)
{
    m_health = 100;
    m_speed = 25.0f;
    m_attackDamage = 15;
}

void FastZombie::update(float deltaTime)
{
    // Update slow timer
    if (m_slowTimer > 0.0f) {
        m_slowTimer -= deltaTime;
        if (m_slowTimer < 0.0f) m_slowTimer = 0.0f;
    }
    
    if (!m_isAttacking) {
        // Move left faster using float precision with effective speed
        m_exactX -= getEffectiveSpeed() * deltaTime;
        m_position.setX(static_cast<int>(m_exactX));
    } else {
        // Decrease attack timer when attacking
        if (m_attackTimer > 0.0f) {
            m_attackTimer -= deltaTime;
        }
    }
}

void FastZombie::render(QPainter *painter)
{
    if (!painter) return;
    
    // Draw a light green zombie (faster)
    painter->setBrush(QColor(144, 238, 144));
    painter->setPen(Qt::black);
    painter->drawEllipse(m_position, 18, 23);
    
    // Draw eyes
    painter->setBrush(Qt::red);
    painter->drawEllipse(m_position + QPoint(-7, -5), 4, 4);
    painter->drawEllipse(m_position + QPoint(7, -5), 4, 4);
}

// Armored Zombie Implementation
ArmoredZombie::ArmoredZombie(QObject *parent)
    : Zombie(parent)
{
    m_health = 400;
    m_speed = 10.0f;
    m_attackDamage = 30;
}

void ArmoredZombie::update(float deltaTime)
{
    // Update slow timer
    if (m_slowTimer > 0.0f) {
        m_slowTimer -= deltaTime;
        if (m_slowTimer < 0.0f) m_slowTimer = 0.0f;
    }
    
    if (!m_isAttacking) {
        // Move left slowly using float precision with effective speed
        m_exactX -= getEffectiveSpeed() * deltaTime;
        m_position.setX(static_cast<int>(m_exactX));
    } else {
        // Decrease attack timer when attacking
        if (m_attackTimer > 0.0f) {
            m_attackTimer -= deltaTime;
        }
    }
}

void ArmoredZombie::render(QPainter *painter)
{
    if (!painter) return;
    
    // Draw a dark gray armored zombie
    painter->setBrush(QColor(64, 64, 64));
    painter->setPen(Qt::black);
    painter->drawEllipse(m_position, 25, 30);
    
    // Draw armor plates
    painter->setBrush(QColor(192, 192, 192));
    painter->drawRect(m_position.x() - 15, m_position.y() - 20, 30, 15);
    
    // Draw eyes
    painter->setBrush(Qt::red);
    painter->drawEllipse(m_position + QPoint(-8, -5), 4, 4);
    painter->drawEllipse(m_position + QPoint(8, -5), 4, 4);
}

// Flag Zombie Implementation
FlagZombie::FlagZombie(QObject *parent)
    : Zombie(parent)
{
    m_health = 250;
    m_speed = 18.0f;
    m_attackDamage = 20;
}

void FlagZombie::update(float deltaTime)
{
    // Update slow timer
    if (m_slowTimer > 0.0f) {
        m_slowTimer -= deltaTime;
        if (m_slowTimer < 0.0f) m_slowTimer = 0.0f;
    }
    
    if (!m_isAttacking) {
        // Move left using float precision with effective speed
        m_exactX -= getEffectiveSpeed() * deltaTime;
        m_position.setX(static_cast<int>(m_exactX));
    } else {
        // Decrease attack timer when attacking
        if (m_attackTimer > 0.0f) {
            m_attackTimer -= deltaTime;
        }
    }
}

void FlagZombie::render(QPainter *painter)
{
    if (!painter) return;
    
    // Draw a brown zombie with flag
    painter->setBrush(QColor(150, 100, 70));
    painter->setPen(Qt::black);
    painter->drawEllipse(m_position, 20, 25);
    
    // Draw flag pole
    painter->setPen(QPen(QColor(139, 69, 19), 2));
    painter->drawLine(m_position.x() + 10, m_position.y() - 25, 
                     m_position.x() + 10, m_position.y() + 10);
    
    // Draw flag
    painter->setBrush(Qt::red);
    painter->setPen(Qt::darkRed);
    QPolygon flagPolygon;
    flagPolygon << QPoint(m_position.x() + 10, m_position.y() - 25)
                << QPoint(m_position.x() + 30, m_position.y() - 18)
                << QPoint(m_position.x() + 10, m_position.y() - 11);
    painter->drawPolygon(flagPolygon);
    
    // Draw eyes
    painter->setBrush(Qt::red);
    painter->drawEllipse(m_position + QPoint(-8, -5), 4, 4);
    painter->drawEllipse(m_position + QPoint(8, -5), 4, 4);
}

// Cone Head Zombie Implementation
ConeHeadZombie::ConeHeadZombie(QObject *parent)
    : Zombie(parent)
{
    m_health = 300;
    m_speed = 14.0f;
    m_attackDamage = 22;
}

void ConeHeadZombie::update(float deltaTime)
{
    // Update slow timer
    if (m_slowTimer > 0.0f) {
        m_slowTimer -= deltaTime;
        if (m_slowTimer < 0.0f) m_slowTimer = 0.0f;
    }
    
    if (!m_isAttacking) {
        // Move left using float precision with effective speed
        m_exactX -= getEffectiveSpeed() * deltaTime;
        m_position.setX(static_cast<int>(m_exactX));
    } else {
        // Decrease attack timer when attacking
        if (m_attackTimer > 0.0f) {
            m_attackTimer -= deltaTime;
        }
    }
}

void ConeHeadZombie::render(QPainter *painter)
{
    if (!painter) return;
    
    // Draw a zombie with orange cone on head
    painter->setBrush(QColor(120, 120, 120));
    painter->setPen(Qt::black);
    painter->drawEllipse(m_position, 20, 25);
    
    // Draw traffic cone on head
    painter->setBrush(QColor(255, 140, 0)); // Orange
    painter->setPen(Qt::darkRed);
    QPolygon conePolygon;
    conePolygon << QPoint(m_position.x(), m_position.y() - 35)
                << QPoint(m_position.x() - 12, m_position.y() - 20)
                << QPoint(m_position.x() + 12, m_position.y() - 20);
    painter->drawPolygon(conePolygon);
    
    // Draw white stripe on cone
    painter->setBrush(Qt::white);
    painter->drawRect(m_position.x() - 10, m_position.y() - 26, 20, 3);
    
    // Draw eyes
    painter->setBrush(Qt::red);
    painter->drawEllipse(m_position + QPoint(-8, -5), 4, 4);
    painter->drawEllipse(m_position + QPoint(8, -5), 4, 4);
}

// Car Zombie Implementation
CarZombie::CarZombie(QObject *parent)
    : Zombie(parent)
    , m_isCrushing(false)
{
    m_health = 500;
    m_speed = 40.0f; // Fast!
    m_attackDamage = 100; // Crushes plants instantly
}

void CarZombie::update(float deltaTime)
{
    // Update slow timer
    if (m_slowTimer > 0.0f) {
        m_slowTimer -= deltaTime;
        if (m_slowTimer < 0.0f) m_slowTimer = 0.0f;
    }
    
    // Always move left - cars don't stop to attack
    m_exactX -= getEffectiveSpeed() * deltaTime;
    m_position.setX(static_cast<int>(m_exactX));
    
    // Car crushes plants by passing over them
    m_isCrushing = true;
}

void CarZombie::render(QPainter *painter)
{
    if (!painter) return;
    
    // Draw a car body - dark gray rectangle
    painter->setBrush(QColor(60, 60, 60));
    painter->setPen(Qt::black);
    painter->drawRect(m_position.x() - 35, m_position.y() - 15, 50, 30);
    
    // Draw car top (cabin)
    painter->setBrush(QColor(80, 80, 80));
    painter->drawRect(m_position.x() - 20, m_position.y() - 25, 25, 15);
    
    // Draw windows
    painter->setBrush(QColor(100, 150, 200));
    painter->drawRect(m_position.x() - 18, m_position.y() - 23, 10, 10);
    painter->drawRect(m_position.x() - 5, m_position.y() - 23, 10, 10);
    
    // Draw wheels
    painter->setBrush(Qt::black);
    painter->drawEllipse(m_position + QPoint(-25, 10), 8, 8);
    painter->drawEllipse(m_position + QPoint(10, 10), 8, 8);
    
    // Draw headlights
    painter->setBrush(Qt::yellow);
    painter->drawEllipse(m_position + QPoint(15, -8), 4, 4);
    painter->drawEllipse(m_position + QPoint(15, 8), 4, 4);
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
