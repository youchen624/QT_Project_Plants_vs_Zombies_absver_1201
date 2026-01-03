#ifndef SHOOTERPLANT_H
#define SHOOTERPLANT_H

#include "plantbase.h"

// Shooter plant (attacks zombies)
class ShooterPlant : public Plant
{
    Q_OBJECT
public:
    explicit ShooterPlant(QObject *parent = nullptr);
    void update(float deltaTime) override;
    void render(QPainter *painter) override;

protected:
    float m_shootTimer;
    float m_shootInterval;
    int m_damage;
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

// Cat Grass plant (shoots tracking projectiles)
class CatGrassPlant : public Plant
{
    Q_OBJECT
public:
    explicit CatGrassPlant(QObject *parent = nullptr);
    void update(float deltaTime) override;
    void render(QPainter *painter) override;

private:
    float m_shootTimer;
    float m_shootInterval;
    int m_damage;
};

// Super Gatling Pea plant (upgrade from Repeater, shoots 15 peas in 3 rows)
class SuperGatlingPeaPlant : public Plant
{
    Q_OBJECT
public:
    explicit SuperGatlingPeaPlant(QObject *parent = nullptr);
    void update(float deltaTime) override;
    void render(QPainter *painter) override;

private:
    float m_shootTimer;
    float m_shootInterval;
    int m_damage;
};

#endif // SHOOTERPLANT_H
