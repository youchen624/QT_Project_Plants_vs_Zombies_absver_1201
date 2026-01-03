#include "Plant.h"
#include <QDebug>

Plant::Plant(QObject *parent)
    : QObject(parent)
    , health(100)
    , maxHealth(100)
    , position(0, 0)
    , cost(50)
    , cooldown(0)
    , currentCooldown(0)
{
}

Plant::~Plant()
{
}

void Plant::setImage(const QString& path)
{
    imagePath = path;
    if (!path.isEmpty()) {
        image = QPixmap(path);
        if (image.isNull()) {
            qWarning() << "Failed to load plant image from:" << path;
        } else {
            qDebug() << "Loaded plant image from:" << path << "Size:" << image.size();
        }
        emit imageChanged();
    }
}

void Plant::takeDamage(int damage)
{
    health -= damage;
    if (health <= 0) {
        health = 0;
        emit died();
    }
}
