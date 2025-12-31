#include "Peashooter.h"

Peashooter::Peashooter(QObject *parent)
    : Plant(parent)
    , shootInterval(90)       // Shoot every 90 ticks (approximately 3 seconds at 30 ticks/sec)
    , ticksSinceLastShot(0)
    , damage(20)              // Each pea does 20 damage
    , range(10)               // Can shoot across 10 grid cells
{
    name = "Peashooter";
    health = 300;
    maxHealth = 300;
    cost = 100;
    cooldown = 240;  // 8 seconds cooldown
    
    // Set default image
    setImage(":/images/peashooter.png");
}

Peashooter::~Peashooter()
{
}

void Peashooter::update(int tickCount)
{
    if (!isAlive()) {
        return;
    }

    ticksSinceLastShot++;

    // Check if it's time to shoot
    if (ticksSinceLastShot >= shootInterval) {
        performAction();
        ticksSinceLastShot = 0;
    }
}

void Peashooter::performAction()
{
    // Shoot a pea in the same row
    int row = position.y();
    emit peaShot(row, damage);
    emit actionPerformed();
}
