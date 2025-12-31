#include "Plant.h"

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

void Plant::takeDamage(int damage)
{
    health -= damage;
    if (health <= 0) {
        health = 0;
        emit died();
    }
}
