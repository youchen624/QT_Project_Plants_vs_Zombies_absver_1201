#include "Projectile.h"
#include <QPen>
#include <QBrush>
#include <QGraphicsScene>

Projectile::Projectile(int startX, int startY, int row, int damage, QGraphicsScene* scene, QObject* parent)
    : QObject(parent)
    , scene(scene)
    , row(row)
    , damage(damage)
    , speed(10)  // Move 10 pixels per step
    , currentX(startX)
    , currentY(startY)
{
    // Create visual representation
    visual = scene->addEllipse(currentX, currentY, 10, 10,
                               QPen(Qt::darkGreen, 1),
                               QBrush(Qt::green));
    
    // Setup movement timer
    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &Projectile::move);
}

Projectile::~Projectile()
{
    if (visual && scene) {
        scene->removeItem(visual);
        delete visual;
    }
}

void Projectile::start()
{
    moveTimer->start(33);  // ~30 FPS for smooth animation
}

int Projectile::getX() const
{
    return currentX;
}

void Projectile::move()
{
    // Move projectile to the right
    currentX += speed;
    visual->setPos(currentX, currentY);
    
    // Check if projectile has gone off screen (right edge at 900 pixels)
    if (currentX > 900) {
        moveTimer->stop();
        emit destroyed();
        deleteLater();
    }
}
