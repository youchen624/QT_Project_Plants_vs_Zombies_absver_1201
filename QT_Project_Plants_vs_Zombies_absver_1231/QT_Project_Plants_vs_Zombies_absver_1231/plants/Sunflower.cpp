#include "Sunflower.h"

Sunflower::Sunflower(QObject *parent)
    : Plant(parent)
    , sunProductionInterval(240)  // Produce sun every 240 ticks (approximately 8 seconds at 30 ticks/sec)
    , ticksSinceLastSun(0)
    , sunAmount(25)               // Each sun produces 25 sun points
{
    name = "Sunflower";
    health = 300;
    maxHealth = 300;
    cost = 50;
    cooldown = 240;  // 8 seconds cooldown
    
    // Set default image
    setImage(":/images/sunflower.png");
}

Sunflower::~Sunflower()
{
}

void Sunflower::update(int tickCount)
{
    if (!isAlive()) {
        return;
    }

    ticksSinceLastSun++;

    // Check if it's time to produce sun
    if (ticksSinceLastSun >= sunProductionInterval) {
        performAction();
        ticksSinceLastSun = 0;
    }
}

void Sunflower::performAction()
{
    // Produce sun
    emit sunProduced(sunAmount);
    emit actionPerformed();
}
