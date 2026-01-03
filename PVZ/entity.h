#ifndef ENTITY_H
#define ENTITY_H

#include <QObject>
#include <QPixmap>
#include <QPoint>

// Base class for all game entities (plants, zombies, projectiles)
class Entity : public QObject
{
    Q_OBJECT
public:
    explicit Entity(QObject *parent = nullptr);
    virtual ~Entity();

    // Virtual methods to be overridden by derived classes
    virtual void update(float deltaTime) = 0;
    virtual void render(QPainter *painter) = 0;
    virtual QRect getBounds() const;

    // Position and state
    QPoint getPosition() const { return m_position; }
    void setPosition(const QPoint &pos) { m_position = pos; }
    
    int getHealth() const { return m_health; }
    void setHealth(int health) { m_health = health; }
    void takeDamage(int damage);
    
    bool isAlive() const { return m_health > 0; }
    bool isMarkedForDeletion() const { return m_markedForDeletion; }
    void markForDeletion() { m_markedForDeletion = true; }

    int getRow() const { return m_row; }
    void setRow(int row) { m_row = row; }
    
    int getCol() const { return m_col; }
    void setCol(int col) { m_col = col; }

signals:
    void entityDestroyed();

protected:
    QPoint m_position;
    int m_health;
    int m_row;
    int m_col;
    bool m_markedForDeletion;
    QPixmap m_sprite;
};

#endif // ENTITY_H
