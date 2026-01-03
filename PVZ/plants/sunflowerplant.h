#ifndef SUNFLOWERPLANT_H
#define SUNFLOWERPLANT_H

#include "plantbase.h"

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

protected:
    float m_produceTimer;
    float m_produceInterval;
};

// Twin Sunflower plant (upgrade from Sunflower, 3x production)
class TwinSunflowerPlant : public Plant
{
    Q_OBJECT
public:
    explicit TwinSunflowerPlant(QObject *parent = nullptr);
    void update(float deltaTime) override;
    void render(QPainter *painter) override;

signals:
    void sunProduced(int amount);

private:
    float m_produceTimer;
    float m_produceInterval;
};

#endif // SUNFLOWERPLANT_H
