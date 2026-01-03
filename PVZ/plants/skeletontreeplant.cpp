#include "skeletontreeplant.h"
#include <QPainter>

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
