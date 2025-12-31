#ifndef PLANTCELL_H
#define PLANTCELL_H

#include <QObject>
#include <QPoint>
#include "Plant.h"

// Represents a grid cell where plants can be placed
class PlantCell : public QObject
{
    Q_OBJECT

public:
    explicit PlantCell(int row, int col, QObject *parent = nullptr);
    ~PlantCell();

    // Getters
    int getRow() const { return row; }
    int getCol() const { return col; }
    QPoint getGridPosition() const { return QPoint(col, row); }
    bool isOccupied() const { return occupied; }
    Plant* getPlant() const { return plant; }

    // Plant management
    bool placePlant(Plant* newPlant);  // Place a plant in this cell
    void removePlant();                 // Remove plant from this cell
    void update(int tickCount);         // Update the plant in this cell

signals:
    void plantPlaced(Plant* plant);     // Emitted when plant is placed
    void plantRemoved();                // Emitted when plant is removed

private slots:
    void onPlantDied();                 // Handle plant death

private:
    int row;                // Row position in grid
    int col;                // Column position in grid
    bool occupied;          // Whether cell has a plant
    Plant* plant;           // Pointer to plant in this cell
};

#endif // PLANTCELL_H
