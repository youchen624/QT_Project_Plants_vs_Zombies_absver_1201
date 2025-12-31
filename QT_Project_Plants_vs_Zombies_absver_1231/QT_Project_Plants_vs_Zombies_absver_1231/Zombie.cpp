#include "Zombie.h"

Zombie::Zombie(QObject *parent)
    : QObject(parent)
    , health(200)
    , maxHealth(200)
    , position(0, 0)
    , speed(1)
    , attackDamage(20)
    , attackCooldown(30)
    , currentAttackCooldown(0)
{
}

Zombie::~Zombie()
{
}

void Zombie::takeDamage(int damage)
{
    health -= damage;
    if (health <= 0) {
        health = 0;
        emit died();
    }
}
