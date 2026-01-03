#ifndef PLANTBASE_H
#define PLANTBASE_H

#include "../entity.h"

// Base class for all plants
class Plant : public Entity
{
    Q_OBJECT
public:
    explicit Plant(QObject *parent = nullptr);
    virtual ~Plant();

    int getCost() const { return m_cost; }
    void setCost(int cost) { m_cost = cost; }

    float getCooldown() const { return m_cooldown; }
    void setCooldown(float cooldown) { m_cooldown = cooldown; }
    
    // Virtual method to check if plant is ignored by zombies
    virtual bool isIgnoredByZombies() const { return false; }
    virtual int getContinuousDamage() const { return 0; }
    
    // Skeleton projectile constants (used by SkeletonTreePlant and GameScene)
    static constexpr int SKELETON_DAMAGE = 30;
    static constexpr float SKELETON_SPEED = 80.0f;

protected:
    int m_cost;           // Sun cost to plant
    float m_cooldown;     // Cooldown time in seconds
};

#endif // PLANTBASE_H
