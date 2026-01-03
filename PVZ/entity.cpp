#include "entity.h"

Entity::Entity(QObject *parent)
    : QObject(parent)
    , m_position(0, 0)
    , m_health(100)
    , m_row(0)
    , m_col(0)
    , m_markedForDeletion(false)
{
}

Entity::~Entity()
{
}

QRect Entity::getBounds() const
{
    if (m_sprite.isNull()) {
        return QRect(m_position, QSize(50, 50)); // Default size
    }
    return QRect(m_position, m_sprite.size());
}

void Entity::takeDamage(int damage)
{
    m_health -= damage;
    if (m_health <= 0) {
        m_health = 0;
        markForDeletion();
        emit entityDestroyed();
    }
}
