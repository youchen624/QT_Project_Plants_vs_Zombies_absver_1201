#ifndef PLANT_H
#define PLANT_H

#include <QObject>
#include <QPoint>
#include <QString>

// Base class for all plants in the game
class Plant : public QObject
{
    Q_OBJECT

public:
    explicit Plant(QObject *parent = nullptr);
    virtual ~Plant();

    // Getters
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    QPoint getPosition() const { return position; }
    int getCost() const { return cost; }
    int getCooldown() const { return cooldown; }
    bool isAlive() const { return health > 0; }
    QString getName() const { return name; }

    // Setters
    void setPosition(const QPoint& pos) { position = pos; }
    void takeDamage(int damage);

    // Virtual methods to be overridden by derived classes
    virtual void update(int tickCount) = 0;  // Update plant state each tick
    virtual void performAction() = 0;         // Perform plant-specific action
    virtual QString getType() const = 0;      // Return plant type

signals:
    void died();                             // Emitted when plant dies
    void actionPerformed();                  // Emitted when plant performs action

protected:
    QString name;           // Plant name
    int health;             // Current health
    int maxHealth;          // Maximum health
    QPoint position;        // Grid position (x, y)
    int cost;               // Sun cost to plant
    int cooldown;           // Cooldown time in ticks
    int currentCooldown;    // Current cooldown counter
};

#endif // PLANT_H
