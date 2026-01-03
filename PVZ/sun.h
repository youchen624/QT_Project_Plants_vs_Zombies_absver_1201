#ifndef SUN_H
#define SUN_H

#include <QObject>
#include <QPoint>
#include <QPainter>

// Sun collectible that falls from sky or produced by sunflowers
class Sun : public QObject
{
    Q_OBJECT
public:
    explicit Sun(const QPoint &startPos, const QPoint &targetPos, bool fromSky, QObject *parent = nullptr);
    ~Sun();

    void update(float deltaTime);
    void render(QPainter *painter);

    QRect getBounds() const;
    bool isCollectable() const { return m_isCollectable; }
    bool isExpired() const { return m_isExpired; }
    int getValue() const { return m_value; }
    void setValue(int value) { m_value = value; }

    void collect() { m_isExpired = true; }

signals:
    void readyToCollect();

private:
    QPoint m_position;
    QPoint m_targetPos;
    bool m_fromSky;
    bool m_isCollectable;
    bool m_isExpired;
    float m_fallSpeed;
    float m_lifeTime;
    float m_maxLifeTime;
    int m_value;
    float m_pulseTimer;
};

#endif // SUN_H
