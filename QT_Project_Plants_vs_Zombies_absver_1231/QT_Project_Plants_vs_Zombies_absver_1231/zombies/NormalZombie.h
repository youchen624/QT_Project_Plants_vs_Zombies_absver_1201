#ifndef NORMALZOMBIE_H
#define NORMALZOMBIE_H

#include "Zombie.h"

// Normal Zombie - basic zombie type
class NormalZombie : public Zombie
{
    Q_OBJECT

public:
    explicit NormalZombie(int row, QObject *parent = nullptr);
    ~NormalZombie() override;

    // Override base class methods
    void update(int tickCount) override;
    void move() override;
    void attack() override;
    QString getType() const override { return "NormalZombie"; }

    // Normal zombie specific
    bool isAttacking() const { return attacking; }
    void setAttacking(bool state) { attacking = state; }

signals:
    void attackingPlant(int row, int damage);  // Emitted when attacking

private:
    bool attacking;            // Whether zombie is currently attacking
    int movementCounter;       // Counter for movement timing
};

#endif // NORMALZOMBIE_H
