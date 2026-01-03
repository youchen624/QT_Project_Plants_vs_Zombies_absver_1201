#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <QObject>
#include <QGraphicsEllipseItem>
#include <QTimer>

// Animated projectile for peas
class Projectile : public QObject
{
    Q_OBJECT

public:
    explicit Projectile(int startX, int startY, int row, int damage, QGraphicsScene* scene, QObject* parent = nullptr);
    ~Projectile();

    void start();
    int getRow() const { return row; }
    int getDamage() const { return damage; }
    int getX() const;

signals:
    void hitTarget(int row, int damage);
    void destroyed();

private slots:
    void move();

private:
    QGraphicsEllipseItem* visual;
    QGraphicsScene* scene;
    QTimer* moveTimer;
    int row;
    int damage;
    int speed;  // pixels per move
    int currentX;
    int currentY;
};

#endif // PROJECTILE_H
