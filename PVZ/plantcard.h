#ifndef PLANTCARD_H
#define PLANTCARD_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

// Plant card widget for selection
class PlantCard : public QWidget
{
    Q_OBJECT
public:
    enum PlantType {
        SUNFLOWER,
        SHOOTER,
        WALL,
        ICESHOOTER,
        REPEATER,
        NUCLEARBOMB,
        SKELETONTREE,
        GROUNDSPIKE,
        CHILIPEPPER,
        MINE,
        TWINSUNFLOWER,
        CATGRASS,
        SUPERGATLINGPEA,
        SUPERNUCLEARBOMB,
        SHOVEL
    };

    explicit PlantCard(PlantType type, int cost, QWidget *parent = nullptr);
    ~PlantCard();

    PlantType getPlantType() const { return m_plantType; }
    int getCost() const { return m_cost; }
    bool isReady() const { return m_cooldownTimer <= 0.0f; }
    bool isSelected() const { return m_selected; }

    void setSelected(bool selected);
    void setEnabled(bool enabled);
    void startCooldown(float duration);
    void updateCooldown(float deltaTime);

signals:
    void clicked(PlantType type);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    PlantType m_plantType;
    int m_cost;
    bool m_selected;
    bool m_enabled;
    float m_cooldownTimer;
    float m_cooldownDuration;

    QString getPlantName() const;
    QColor getPlantColor() const;
};

#endif // PLANTCARD_H
