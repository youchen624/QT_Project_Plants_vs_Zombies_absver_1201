#ifndef ZOMBIEBASE_H
#define ZOMBIEBASE_H

#include "../entity.h"

// Base class for all zombies
class Zombie : public Entity
{
    Q_OBJECT
public:
    explicit Zombie(QObject *parent = nullptr);
    virtual ~Zombie();

    float getSpeed() const { return m_speed; }
    void setSpeed(float speed) { m_speed = speed; }

    int getAttackDamage() const { return m_attackDamage; }
    void setAttackDamage(int damage) { m_attackDamage = damage; }

    float getAttackInterval() const { return m_attackInterval; }
    void setAttackInterval(float interval) { m_attackInterval = interval; }

    bool isAttacking() const { return m_isAttacking; }
    void setAttacking(bool attacking) { m_isAttacking = attacking; }

    float getAttackTimer() const { return m_attackTimer; }
    void resetAttackTimer() { m_attackTimer = 0.0f; }
    void updateAttackTimer(float deltaTime) { m_attackTimer += deltaTime; }
    
    void syncExactPosition() { m_exactX = static_cast<float>(m_position.x()); }
    
    // Slow/freeze effect
    void applySlow(float duration = 3.0f);
    bool isSlowed() const { return m_slowTimer > 0.0f; }
    float getEffectiveSpeed() const;
    
    // Pole vaulting
    virtual bool canVault() const { return false; }
    virtual bool hasVaulted() const { return false; }
    virtual void setVaulted(bool vaulted) { }

protected:
    float m_speed;           // Movement speed (pixels per second)
    int m_attackDamage;      // Damage per attack
    float m_attackInterval;  // Time between attacks
    float m_attackTimer;     // Current attack timer
    bool m_isAttacking;      // Whether currently attacking
    float m_exactX;          // Exact X position as float for smooth movement
    float m_slowTimer;       // Timer for slow effect
    float m_slowMultiplier;  // Speed multiplier when slowed (0.5 = 50% speed)
};

// Skeleton - friendly entity that attacks zombies (summoned by Skeleton Tree)
class Skeleton : public Entity
{
    Q_OBJECT
public:
    explicit Skeleton(QObject *parent = nullptr);
    void update(float deltaTime) override;
    void render(QPainter *painter) override;
    
    float getSpeed() const { return m_speed; }
    void setSpeed(float speed) { m_speed = speed; }
    void setDirection(int direction) { m_direction = direction; } // -1 for left, 1 for right
    
    int getAttackDamage() const { return m_attackDamage; }
    bool isAttacking() const { return m_isAttacking; }
    void setAttacking(bool attacking) { m_isAttacking = attacking; }
    
    float getAttackTimer() const { return m_attackTimer; }
    void resetAttackTimer() { m_attackTimer = 0.0f; }
    void updateAttackTimer(float deltaTime) { m_attackTimer += deltaTime; }
    
    void syncExactPosition() { m_exactX = static_cast<float>(m_position.x()); }

private:
    float m_speed;           // Movement speed
    int m_direction;         // -1 for left (attacking), 1 for right
    int m_attackDamage;      // Damage per attack
    float m_attackInterval;  // Time between attacks
    float m_attackTimer;     // Current attack timer
    bool m_isAttacking;      // Whether currently attacking
    float m_exactX;          // Exact X position as float for smooth movement
};

#endif // ZOMBIEBASE_H
