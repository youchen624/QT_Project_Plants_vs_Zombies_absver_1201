#ifndef BOMBPLANT_H
#define BOMBPLANT_H

#include "plantbase.h"

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

// Super Nuclear Bomb plant (destroys ALL plants and zombies with animation)
class SuperNuclearBombPlant : public Plant
{
    Q_OBJECT
public:
    explicit SuperNuclearBombPlant(QObject *parent = nullptr);
    void update(float deltaTime) override;
    void render(QPainter *painter) override;

signals:
    void superExplode();

private:
    float m_detonateTimer;
    float m_detonateDelay;
    bool m_hasExploded;
    float m_animationTimer;
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

#endif // BOMBPLANT_H
