#ifndef DEFENSIVEPLANT_H
#define DEFENSIVEPLANT_H

#include "plantbase.h"

// Wall plant (blocks zombies)
class WallPlant : public Plant
{
    Q_OBJECT
public:
    explicit WallPlant(QObject *parent = nullptr);
    void update(float deltaTime) override;
    void render(QPainter *painter) override;
};

// Ground Spike plant (ignored by zombies, deals continuous damage)
class GroundSpikePlant : public Plant
{
    Q_OBJECT
public:
    explicit GroundSpikePlant(QObject *parent = nullptr);
    void update(float deltaTime) override;
    void render(QPainter *painter) override;
    
    bool isIgnoredByZombies() const override { return true; } // Special flag
    int getContinuousDamage() const override { return m_damage; }

private:
    int m_damage;
};

#endif // DEFENSIVEPLANT_H
