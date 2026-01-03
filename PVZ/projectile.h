#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "entity.h"

// Projectile class for bullets/peas shot by plants
class Projectile : public Entity
{
    Q_OBJECT
public:
    enum ProjectileType {
        NORMAL,
        ICE,
        TRACKING
    };

    explicit Projectile(int damage, float speed, ProjectileType type = NORMAL, QObject *parent = nullptr);
    virtual ~Projectile();

    void update(float deltaTime) override;
    void render(QPainter *painter) override;

    int getDamage() const { return m_damage; }
    float getSpeed() const { return m_speed; }
    ProjectileType getType() const { return m_type; }
    bool isIceProjectile() const { return m_type == ICE; }
    bool isTrackingProjectile() const { return m_type == TRACKING; }
    
    void setTarget(Entity* target) { m_target = target; }
    Entity* getTarget() const { return m_target; }

private:
    int m_damage;
    float m_speed;
    ProjectileType m_type;
    Entity* m_target; // Target for tracking projectiles
    float m_exactX;
    float m_exactY;
};

#endif // PROJECTILE_H
