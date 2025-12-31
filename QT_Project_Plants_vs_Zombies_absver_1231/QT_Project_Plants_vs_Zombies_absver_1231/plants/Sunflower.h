#ifndef SUNFLOWER_H
#define SUNFLOWER_H

#include "Plant.h"

// Sunflower plant - generates sun resources
class Sunflower : public Plant
{
    Q_OBJECT

public:
    explicit Sunflower(QObject *parent = nullptr);
    ~Sunflower() override;

    // Override base class methods
    void update(int tickCount) override;
    void performAction() override;
    QString getType() const override { return "Sunflower"; }

    // Sunflower specific
    int getSunProductionInterval() const { return sunProductionInterval; }
    int getSunAmount() const { return sunAmount; }

signals:
    void sunProduced(int amount);  // Emitted when sun is produced

private:
    int sunProductionInterval;     // Ticks between sun production
    int ticksSinceLastSun;         // Counter for sun production
    int sunAmount;                 // Amount of sun produced
};

#endif // SUNFLOWER_H
