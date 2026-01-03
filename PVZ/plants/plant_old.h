#ifndef PLANT_H
#define PLANT_H

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

// Shooter plant (attacks zombies)
class ShooterPlant : public Plant
{
    Q_OBJECT
public:
    explicit ShooterPlant(QObject *parent = nullptr);
    void update(float deltaTime) override;
    void render(QPainter *painter) override;

private:
    float m_shootTimer;
    float m_shootInterval;
    int m_damage;
};

// Wall plant (blocks zombies)
class WallPlant : public Plant
{
    Q_OBJECT
public:
    explicit WallPlant(QObject *parent = nullptr);
    void update(float deltaTime) override;
    void render(QPainter *painter) override;
};

// Sun generator plant (produces sun resources)
class SunflowerPlant : public Plant
{
    Q_OBJECT
public:
    explicit SunflowerPlant(QObject *parent = nullptr);
    void update(float deltaTime) override;
    void render(QPainter *painter) override;

signals:
    void sunProduced(int amount);

private:
    float m_produceTimer;
    float m_produceInterval;
};

// Ice Shooter plant (shoots ice projectiles that slow zombies)
class IceShooterPlant : public Plant
{
    Q_OBJECT
public:
    explicit IceShooterPlant(QObject *parent = nullptr);
    void update(float deltaTime) override;
    void render(QPainter *painter) override;

private:
    float m_shootTimer;
    float m_shootInterval;
    int m_damage;
};

// Repeater plant (shoots two peas at a time for double damage)
class RepeaterPlant : public Plant
{
    Q_OBJECT
public:
    explicit RepeaterPlant(QObject *parent = nullptr);
    void update(float deltaTime) override;
    void render(QPainter *painter) override;

private:
    float m_shootTimer;
    float m_shootInterval;
    int m_damage;
};

// Nuclear Bomb plant (clears all zombies on screen after a delay)
class NuclearBombPlant : public Plant
{
    Q_OBJECT
public:
    explicit NuclearBombPlant(QObject *parent = nullptr);
    void update(float deltaTime) override;
    void render(QPainter *painter) override;

signals:
    void explode();

private:
    float m_detonateTimer;
    float m_detonateDelay;
    bool m_hasExploded;
};

// Skeleton Tree plant (summons skeletons on upper/lower lanes, not current lane)
class SkeletonTreePlant : public Plant
{
    Q_OBJECT
public:
    explicit SkeletonTreePlant(QObject *parent = nullptr);
    void update(float deltaTime) override;
    void render(QPainter *painter) override;

signals:
    void summonSkeleton(int treeRow); // Emit tree's row to trigger skeleton summoning on adjacent lanes

private:
    float m_summonTimer;
    float m_summonInterval;
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

// Chili Pepper plant (burns all zombies in row when activated)
class ChiliPepperPlant : public Plant
{
    Q_OBJECT
public:
    explicit ChiliPepperPlant(QObject *parent = nullptr);
    void update(float deltaTime) override;
    void render(QPainter *painter) override;

signals:
    void burnRow(int row);

private:
    float m_activateTimer;
    float m_activateDelay;
    bool m_hasActivated;
};

// Mine plant (consumable, explodes when zombie steps on it)
class MinePlant : public Plant
{
    Q_OBJECT
public:
    explicit MinePlant(QObject *parent = nullptr);
    void update(float deltaTime) override;
    void render(QPainter *painter) override;
    
    bool isIgnoredByZombies() const override { return true; } // Zombies walk over it
    bool shouldExplode() const { return m_shouldExplode; }
    void triggerExplosion() { m_shouldExplode = true; }

signals:
    void explode(int row, int col);

private:
    bool m_shouldExplode;
    float m_explodeTimer;
    float m_explodeDelay;
};

#endif // PLANT_H
