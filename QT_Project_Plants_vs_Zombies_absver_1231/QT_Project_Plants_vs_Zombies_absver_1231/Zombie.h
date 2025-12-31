#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <QObject>
#include <QPoint>
#include <QString>

// Base class for all zombies in the game
class Zombie : public QObject
{
    Q_OBJECT

public:
    explicit Zombie(QObject *parent = nullptr);
    virtual ~Zombie();

    // Getters
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    QPoint getPosition() const { return position; }
    int getSpeed() const { return speed; }
    int getAttackDamage() const { return attackDamage; }
    bool isAlive() const { return health > 0; }
    QString getName() const { return name; }

    // Setters
    void setPosition(const QPoint& pos) { position = pos; }
    void takeDamage(int damage);

    // Virtual methods to be overridden by derived classes
    virtual void update(int tickCount) = 0;   // Update zombie state each tick
    virtual void move() = 0;                  // Move zombie
    virtual void attack() = 0;                // Attack plants
    virtual QString getType() const = 0;      // Return zombie type

signals:
    void died();                              // Emitted when zombie dies
    void reachedEnd();                        // Emitted when zombie reaches the end

protected:
    QString name;           // Zombie name
    int health;             // Current health
    int maxHealth;          // Maximum health
    QPoint position;        // Current position (x, y)
    int speed;              // Movement speed (pixels per tick)
    int attackDamage;       // Damage per attack
    int attackCooldown;     // Attack cooldown in ticks
    int currentAttackCooldown; // Current attack cooldown counter
};

#endif // ZOMBIE_H
