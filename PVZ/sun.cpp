#include "sun.h"
#include <cmath>

Sun::Sun(const QPoint &startPos, const QPoint &targetPos, bool fromSky, QObject *parent)
    : QObject(parent)
    , m_position(startPos)
    , m_targetPos(targetPos)
    , m_fromSky(fromSky)
    , m_isCollectable(true)
    , m_isExpired(false)
    , m_fallSpeed(100.0f)
    , m_lifeTime(0.0f)
    , m_maxLifeTime(8.0f)
    , m_value(25)
    , m_pulseTimer(0.0f)
{
}

Sun::~Sun()
{
}

void Sun::update(float deltaTime)
{
    if (m_isExpired) return;

    m_lifeTime += deltaTime;
    m_pulseTimer += deltaTime;

    // Fall to target position
    float dx = m_targetPos.x() - m_position.x();
    float dy = m_targetPos.y() - m_position.y();
    float distance = sqrt(dx * dx + dy * dy);

    if (distance < 5.0f) {
        m_position = m_targetPos;
    } else {
        float moveDistance = m_fallSpeed * deltaTime;
        if (moveDistance > distance) {
            moveDistance = distance;
        }
        
        // Check for near-zero distance to avoid division by zero
        if (distance > 0.01f) {
            m_position.setX(m_position.x() + static_cast<int>(dx / distance * moveDistance));
            m_position.setY(m_position.y() + static_cast<int>(dy / distance * moveDistance));
        } else {
            m_position = m_targetPos;
        }
    }

    // Expire after max lifetime
    if (m_lifeTime >= m_maxLifeTime) {
        m_isExpired = true;
    }
}

void Sun::render(QPainter *painter)
{
    if (!painter || m_isExpired) return;

    // Pulsing effect
    float pulseScale = 1.0f + 0.1f * sin(m_pulseTimer * 3.0f);
    int radius = static_cast<int>(20 * pulseScale);

    // Outer glow
    QRadialGradient gradient(m_position, radius + 5);
    gradient.setColorAt(0, QColor(255, 255, 150, 200));
    gradient.setColorAt(0.7, QColor(255, 255, 0, 150));
    gradient.setColorAt(1, QColor(255, 255, 0, 0));
    
    painter->setBrush(gradient);
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(m_position, radius + 5, radius + 5);

    // Main sun body
    QRadialGradient sunGradient(m_position, radius);
    sunGradient.setColorAt(0, QColor(255, 255, 200));
    sunGradient.setColorAt(0.5, QColor(255, 255, 0));
    sunGradient.setColorAt(1, QColor(255, 200, 0));
    
    painter->setBrush(sunGradient);
    painter->setPen(QPen(QColor(255, 200, 0), 2));
    painter->drawEllipse(m_position, radius, radius);

    // Shine effect
    painter->setBrush(QColor(255, 255, 255, 100));
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(m_position + QPoint(-5, -5), 8, 8);
}

QRect Sun::getBounds() const
{
    return QRect(m_position - QPoint(20, 20), QSize(40, 40));
}
