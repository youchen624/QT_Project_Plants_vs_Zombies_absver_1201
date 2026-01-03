#ifndef PEASHOOTER_H
#define PEASHOOTER_H

#include "Plant.h"

// Peashooter plant - shoots peas at zombies
class Peashooter : public Plant
{
    Q_OBJECT

public:
    explicit Peashooter(QObject *parent = nullptr);
    ~Peashooter() override;

    // Override base class methods
    void update(int tickCount) override;
    void performAction() override;
    QString getType() const override { return "Peashooter"; }

    // Peashooter specific
    int getShootInterval() const { return shootInterval; }
    int getDamage() const { return damage; }
    int getRange() const { return range; }

signals:
    void peaShot(int row, int damage);  // Emitted when pea is shot

private:
    int shootInterval;         // Ticks between shots
    int ticksSinceLastShot;    // Counter for shooting
    int damage;                // Damage per pea
    int range;                 // Attack range (in grid cells)
};

#endif // PEASHOOTER_H
