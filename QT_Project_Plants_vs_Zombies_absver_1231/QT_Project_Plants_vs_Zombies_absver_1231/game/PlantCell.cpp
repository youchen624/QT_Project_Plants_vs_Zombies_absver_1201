#include "PlantCell.h"

PlantCell::PlantCell(int row, int col, QObject *parent)
    : QObject(parent)
    , row(row)
    , col(col)
    , occupied(false)
    , plant(nullptr)
{
}

PlantCell::~PlantCell()
{
    if (plant) {
        delete plant;
        plant = nullptr;
    }
}

bool PlantCell::placePlant(Plant* newPlant)
{
    if (occupied || !newPlant) {
        return false;
    }

    plant = newPlant;
    plant->setPosition(QPoint(col, row));
    plant->setParent(this);
    occupied = true;

    // Connect plant death signal
    connect(plant, &Plant::died, this, &PlantCell::onPlantDied);

    emit plantPlaced(plant);
    return true;
}

void PlantCell::removePlant()
{
    if (plant) {
        disconnect(plant, &Plant::died, this, &PlantCell::onPlantDied);
        plant->deleteLater();
        plant = nullptr;
        occupied = false;
        emit plantRemoved();
    }
}

void PlantCell::update(int tickCount)
{
    if (occupied && plant && plant->isAlive()) {
        plant->update(tickCount);
    }
}

void PlantCell::onPlantDied()
{
    removePlant();
}
