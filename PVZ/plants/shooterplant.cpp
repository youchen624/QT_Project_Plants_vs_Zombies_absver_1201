#include "shooterplant.h"
#include <QPainter>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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
        float angle = i * 90.0f * static_cast<float>(M_PI) / 180.0f;
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

// Cat Grass Plant Implementation
CatGrassPlant::CatGrassPlant(QObject *parent)
    : Plant(parent)
    , m_shootTimer(0.0f)
    , m_shootInterval(2.2f)
    , m_damage(25)
{
    m_health = 180;
    m_cost = 225;
    m_cooldown = 8.0f;
}

void CatGrassPlant::update(float deltaTime)
{
    m_shootTimer += deltaTime;
    // Shooting logic will be handled by game scene
}

void CatGrassPlant::render(QPainter *painter)
{
    if (!painter) return;
    
    // Draw a purple/pink cat-like plant
    painter->setBrush(QColor(200, 100, 200));
    painter->setPen(QColor(150, 50, 150));
    painter->drawEllipse(m_position, 25, 25);
    
    // Draw cat ears
    painter->setBrush(QColor(180, 80, 180));
    QPolygon leftEar;
    leftEar << QPoint(m_position.x() - 15, m_position.y() - 25)
            << QPoint(m_position.x() - 20, m_position.y() - 35)
            << QPoint(m_position.x() - 10, m_position.y() - 30);
    painter->drawPolygon(leftEar);
    
    QPolygon rightEar;
    rightEar << QPoint(m_position.x() + 15, m_position.y() - 25)
             << QPoint(m_position.x() + 20, m_position.y() - 35)
             << QPoint(m_position.x() + 10, m_position.y() - 30);
    painter->drawPolygon(rightEar);
    
    // Draw eyes
    painter->setBrush(Qt::white);
    painter->drawEllipse(m_position + QPoint(-8, -5), 6, 8);
    painter->drawEllipse(m_position + QPoint(8, -5), 6, 8);
    
    // Draw pupils
    painter->setBrush(Qt::black);
    painter->drawEllipse(m_position + QPoint(-8, -3), 3, 4);
    painter->drawEllipse(m_position + QPoint(8, -3), 3, 4);
}

// Super Gatling Pea Plant Implementation
SuperGatlingPeaPlant::SuperGatlingPeaPlant(QObject *parent)
    : Plant(parent)
    , m_shootTimer(0.0f)
    , m_shootInterval(1.5f)
    , m_damage(300) // 15 peas x 20 damage each
{
    m_health = 250;
    m_cost = 800;
    m_cooldown = 30.0f;
}

void SuperGatlingPeaPlant::update(float deltaTime)
{
    m_shootTimer += deltaTime;
    // Shooting logic will be handled by game scene
}

void SuperGatlingPeaPlant::render(QPainter *painter)
{
    if (!painter) return;
    
    // Draw a very dark green circle for super gatling
    painter->setBrush(QColor(0, 100, 0));
    painter->setPen(Qt::black);
    painter->drawEllipse(m_position, 30, 30);
    
    // Draw multiple shooter indicators in 3 rows
    painter->setBrush(Qt::yellow);
    // Top row
    for (int i = 0; i < 3; i++) {
        painter->drawEllipse(m_position + QPoint(15 + i*3, -10), 5, 5);
    }
    // Middle row
    for (int i = 0; i < 3; i++) {
        painter->drawEllipse(m_position + QPoint(15 + i*3, 0), 5, 5);
    }
    // Bottom row
    for (int i = 0; i < 3; i++) {
        painter->drawEllipse(m_position + QPoint(15 + i*3, 10), 5, 5);
    }
    
    // Draw "SUPER" text
    painter->setFont(QFont("Arial", 6, QFont::Bold));
    painter->setPen(Qt::red);
    painter->drawText(m_position.x() - 15, m_position.y() + 5, "SUPER");
}
