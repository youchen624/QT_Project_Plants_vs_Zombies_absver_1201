#include "NormalZombie.h"

// Zombie spawn position constant
static const int SPAWN_X_POSITION = 900;  // Right edge of the game field

NormalZombie::NormalZombie(int row, QObject *parent)
    : Zombie(parent)
    , attacking(false)
    , movementCounter(0)
{
    name = "Normal Zombie";
    health = 200;
    maxHealth = 200;
    speed = 1;               // Moves 1 pixel per tick
    attackDamage = 20;       // Does 20 damage per attack
    attackCooldown = 30;     // Attacks every 30 ticks (approximately 1 second)
    currentAttackCooldown = 0;
    
    // Set initial position at the right edge of the grid for the given row
    position = QPoint(SPAWN_X_POSITION, row);
    
    // Set default image
    setImage(":/images/zombie.png");
}

NormalZombie::~NormalZombie()
{
}

void NormalZombie::update(int tickCount)
{
    if (!isAlive()) {
        return;
    }

    if (attacking) {
        // If attacking, don't move but manage attack cooldown
        if (currentAttackCooldown <= 0) {
            // Ready to attack - emit signal
            emit attackingPlant(position.y(), attackDamage);
            currentAttackCooldown = attackCooldown;
        } else {
            currentAttackCooldown--;
        }
    } else {
        // If not attacking, move forward
        move();
        // Reset attack cooldown when starting to move again
        currentAttackCooldown = 0;
    }

    // Check if reached the left edge (end of level)
    if (position.x() <= 0) {
        emit reachedEnd();
    }
}

void NormalZombie::move()
{
    movementCounter++;
    
    // Move every 2 ticks to control speed
    if (movementCounter >= 2) {
        position.setX(position.x() - speed);
        movementCounter = 0;
    }
}

void NormalZombie::attack()
{
    // Emit signal that zombie is attacking plants in its row
    emit attackingPlant(position.y(), attackDamage);
}
