#include "plantcard.h"
#include <QFont>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

PlantCard::PlantCard(PlantType type, int cost, QWidget *parent)
    : QWidget(parent)
    , m_plantType(type)
    , m_cost(cost)
    , m_selected(false)
    , m_enabled(true)
    , m_cooldownTimer(0.0f)
    , m_cooldownDuration(0.0f)
{
    setFixedSize(80, 100);
    setCursor(Qt::PointingHandCursor);
}

PlantCard::~PlantCard()
{
}

void PlantCard::setSelected(bool selected)
{
    m_selected = selected;
    update();
}

void PlantCard::setEnabled(bool enabled)
{
    m_enabled = enabled;
    update();
}

void PlantCard::startCooldown(float duration)
{
    m_cooldownTimer = duration;
    m_cooldownDuration = duration;
}

void PlantCard::updateCooldown(float deltaTime)
{
    if (m_cooldownTimer > 0.0f) {
        m_cooldownTimer -= deltaTime;
        if (m_cooldownTimer < 0.0f) {
            m_cooldownTimer = 0.0f;
        }
        update();
    }
}

QString PlantCard::getPlantName() const
{
    switch (m_plantType) {
    case SUNFLOWER:
        return "太陽花";
    case SHOOTER:
        return "豌豆射手";
    case WALL:
        return "牆壁";
    case ICESHOOTER:
        return "寒冰射手";
    case REPEATER:
        return "雙倍豌豆射手";
    case NUCLEARBOMB:
        return "核彈";
    case SKELETONTREE:
        return "骷髏樹";
    case GROUNDSPIKE:
        return "地刺";
    case CHILIPEPPER:
        return "火爆爆辣椒";
    case MINE:
        return "地雷";
    case TWINSUNFLOWER:
        return "雙倍太陽花";
    case CATGRASS:
        return "細菌草";
    case SUPERGATLINGPEA:
        return "超級無敵宇宙霹靂\n究極豌豆射手";
    case SUPERNUCLEARBOMB:
        return "\"我C你M\"彈";
    case SHOVEL:
        return "鏟子";
    default:
        return "Unknown";
    }
}

QColor PlantCard::getPlantColor() const
{
    switch (m_plantType) {
    case SUNFLOWER:
        return Qt::yellow;
    case SHOOTER:
        return Qt::green;
    case WALL:
        return QColor(139, 69, 19);
    case ICESHOOTER:
        return QColor(100, 200, 255);
    case REPEATER:
        return QColor(0, 150, 0);
    case NUCLEARBOMB:
        return QColor(255, 69, 0); // Orange-red
    case SKELETONTREE:
        return QColor(80, 50, 30); // Dark brown
    case GROUNDSPIKE:
        return QColor(150, 150, 150); // Gray
    case CHILIPEPPER:
        return QColor(200, 0, 0); // Red
    case MINE:
        return QColor(50, 50, 50); // Dark gray/black
    case TWINSUNFLOWER:
        return QColor(255, 215, 0); // Gold
    case CATGRASS:
        return QColor(200, 100, 200); // Purple/pink
    case SUPERGATLINGPEA:
        return QColor(0, 100, 0); // Very dark green
    case SUPERNUCLEARBOMB:
        return QColor(255, 0, 0); // Bright red
    case SHOVEL:
        return QColor(192, 192, 192); // Silver/gray
    default:
        return Qt::gray;
    }
}

void PlantCard::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Card background
    QColor bgColor = m_enabled ? Qt::white : QColor(200, 200, 200);
    if (m_selected) {
        bgColor = QColor(173, 216, 230); // Light blue for selected
    }
    
    painter.setBrush(bgColor);
    painter.setPen(QPen(Qt::black, 2));
    painter.drawRoundedRect(5, 5, width() - 10, height() - 10, 5, 5);

    // Plant icon area
    QRect iconRect(10, 10, 60, 60);
    painter.setBrush(getPlantColor());
    painter.setPen(Qt::black);
    
    switch (m_plantType) {
    case SUNFLOWER:
        painter.drawEllipse(iconRect.center(), 25, 25);
        // Draw petals
        painter.setBrush(QColor(255, 200, 0));
        for (int i = 0; i < 6; i++) {
            float angle = i * 60.0f * 3.14159f / 180.0f;
            int x = iconRect.center().x() + static_cast<int>(18 * cos(angle));
            int y = iconRect.center().y() + static_cast<int>(18 * sin(angle));
            painter.drawEllipse(QPoint(x, y), 8, 8);
        }
        break;
    case SHOOTER:
        painter.drawEllipse(iconRect.center(), 22, 22);
        painter.setBrush(Qt::yellow);
        painter.drawEllipse(iconRect.center() + QPoint(12, 0), 6, 6);
        break;
    case WALL:
        painter.drawRect(iconRect.adjusted(10, 10, -10, -10));
        break;
    case ICESHOOTER:
        painter.drawEllipse(iconRect.center(), 22, 22);
        painter.setBrush(QColor(200, 230, 255));
        // Draw snowflake
        for (int i = 0; i < 4; i++) {
            float angle = i * 90.0f * static_cast<float>(M_PI) / 180.0f;
            int x1 = iconRect.center().x() + static_cast<int>(15 * cos(angle));
            int y1 = iconRect.center().y() + static_cast<int>(15 * sin(angle));
            painter.drawLine(iconRect.center(), QPoint(x1, y1));
        }
        break;
    case REPEATER:
        painter.drawEllipse(iconRect.center(), 22, 22);
        painter.setBrush(Qt::yellow);
        painter.drawEllipse(iconRect.center() + QPoint(10, -5), 5, 5);
        painter.drawEllipse(iconRect.center() + QPoint(10, 5), 5, 5);
        break;
    case NUCLEARBOMB:
        // Draw nuclear bomb
        painter.drawEllipse(iconRect.center(), 20, 20);
        painter.setBrush(Qt::yellow);
        painter.drawEllipse(iconRect.center(), 6, 6);
        // Draw radiation symbol
        painter.setPen(QPen(Qt::black, 2));
        for (int i = 0; i < 3; i++) {
            float angle = i * 120.0f * static_cast<float>(M_PI) / 180.0f;
            int x1 = iconRect.center().x() + static_cast<int>(12 * cos(angle));
            int y1 = iconRect.center().y() + static_cast<int>(12 * sin(angle));
            painter.drawLine(iconRect.center(), QPoint(x1, y1));
        }
        break;
    case SKELETONTREE:
        // Draw skeleton tree
        painter.drawRect(iconRect.center().x() - 5, iconRect.center().y() - 5, 10, 20); // Trunk
        painter.setBrush(QColor(220, 220, 220)); // Skull
        painter.drawEllipse(iconRect.center() + QPoint(0, -8), 12, 10);
        painter.setBrush(Qt::black);
        painter.drawEllipse(iconRect.center() + QPoint(-5, -10), 3, 4); // Eye
        painter.drawEllipse(iconRect.center() + QPoint(5, -10), 3, 4); // Eye
        break;
    case GROUNDSPIKE:
        // Draw ground spike
        painter.drawEllipse(iconRect.center(), 18, 8); // Base
        painter.setBrush(QColor(180, 180, 180));
        // Draw spikes
        for (int i = -1; i <= 1; i++) {
            QPolygon spike;
            int xOffset = i * 8;
            spike << QPoint(iconRect.center().x() + xOffset - 3, iconRect.center().y())
                  << QPoint(iconRect.center().x() + xOffset + 3, iconRect.center().y())
                  << QPoint(iconRect.center().x() + xOffset, iconRect.center().y() - 12);
            painter.drawPolygon(spike);
        }
        break;
    case CHILIPEPPER:
        // Draw chili pepper
        painter.drawEllipse(iconRect.center(), 8, 18); // Chili body
        painter.setBrush(Qt::darkGreen);
        painter.drawRect(iconRect.center().x() - 2, iconRect.center().y() - 22, 4, 6); // Stem
        break;
    case MINE:
        // Draw mine
        painter.drawEllipse(iconRect.center(), 18, 18); // Mine body
        painter.setBrush(QColor(100, 100, 100)); // Gray
        // Draw spikes around mine
        for (int i = 0; i < 6; i++) {
            float angle = i * 60.0f * static_cast<float>(M_PI) / 180.0f;
            int x1 = iconRect.center().x() + static_cast<int>(12 * cos(angle));
            int y1 = iconRect.center().y() + static_cast<int>(12 * sin(angle));
            int x2 = iconRect.center().x() + static_cast<int>(18 * cos(angle));
            int y2 = iconRect.center().y() + static_cast<int>(18 * sin(angle));
            painter.drawLine(QPoint(x1, y1), QPoint(x2, y2));
        }
        // Draw warning symbol
        painter.setBrush(Qt::red);
        painter.drawEllipse(iconRect.center(), 5, 5);
        break;
    case TWINSUNFLOWER:
        // Draw twin sunflower (two sunflowers side by side)
        painter.drawEllipse(iconRect.center() + QPoint(-10, 0), 18, 18);
        painter.drawEllipse(iconRect.center() + QPoint(10, 0), 18, 18);
        // Draw petals for both
        painter.setBrush(QColor(255, 200, 0));
        for (int i = 0; i < 6; i++) {
            float angle = i * 60.0f * static_cast<float>(M_PI) / 180.0f;
            // Left sunflower petals
            int x = iconRect.center().x() - 10 + static_cast<int>(14 * cos(angle));
            int y = iconRect.center().y() + static_cast<int>(14 * sin(angle));
            painter.drawEllipse(QPoint(x, y), 6, 6);
            // Right sunflower petals
            x = iconRect.center().x() + 10 + static_cast<int>(14 * cos(angle));
            painter.drawEllipse(QPoint(x, y), 6, 6);
        }
        break;
    case CATGRASS:
    {
        // Draw cat grass (grass with cat ears)
        painter.drawEllipse(iconRect.center(), 20, 22); // Main body
        painter.setBrush(QColor(180, 80, 180)); // Darker purple
        // Draw cat ears
        QPolygon leftEar;
        leftEar << QPoint(iconRect.center().x() - 12, iconRect.center().y() - 15)
                << QPoint(iconRect.center().x() - 18, iconRect.center().y() - 25)
                << QPoint(iconRect.center().x() - 6, iconRect.center().y() - 20);
        painter.drawPolygon(leftEar);
        QPolygon rightEar;
        rightEar << QPoint(iconRect.center().x() + 12, iconRect.center().y() - 15)
                 << QPoint(iconRect.center().x() + 18, iconRect.center().y() - 25)
                 << QPoint(iconRect.center().x() + 6, iconRect.center().y() - 20);
        painter.drawPolygon(rightEar);
        // Draw eyes
        painter.setBrush(Qt::white);
        painter.drawEllipse(iconRect.center() + QPoint(-7, -5), 4, 5);
        painter.drawEllipse(iconRect.center() + QPoint(7, -5), 4, 5);
        painter.setBrush(Qt::black);
        painter.drawEllipse(iconRect.center() + QPoint(-7, -3), 2, 3);
        painter.drawEllipse(iconRect.center() + QPoint(7, -3), 2, 3);
        break;
    }
    case SUPERGATLINGPEA:
        // Draw super gatling pea (large pea with triple barrels)
        painter.drawEllipse(iconRect.center(), 24, 24);
        painter.setBrush(Qt::yellow);
        // Draw three projectiles
        painter.drawEllipse(iconRect.center() + QPoint(15, -8), 5, 5);
        painter.drawEllipse(iconRect.center() + QPoint(15, 0), 5, 5);
        painter.drawEllipse(iconRect.center() + QPoint(15, 8), 5, 5);
        // Draw lines showing multiple barrels
        painter.setPen(QPen(Qt::black, 2));
        painter.drawLine(iconRect.center() + QPoint(5, -8), iconRect.center() + QPoint(12, -8));
        painter.drawLine(iconRect.center() + QPoint(5, 0), iconRect.center() + QPoint(12, 0));
        painter.drawLine(iconRect.center() + QPoint(5, 8), iconRect.center() + QPoint(12, 8));
        break;
    case SUPERNUCLEARBOMB:
        // Draw super nuclear bomb (larger with more dramatic effect)
        painter.drawEllipse(iconRect.center(), 24, 24);
        painter.setBrush(QColor(255, 100, 0)); // Orange
        painter.drawEllipse(iconRect.center(), 18, 18);
        painter.setBrush(Qt::yellow);
        painter.drawEllipse(iconRect.center(), 8, 8);
        // Draw radiation symbols
        painter.setPen(QPen(Qt::black, 3));
        for (int i = 0; i < 3; i++) {
            float angle = i * 120.0f * static_cast<float>(M_PI) / 180.0f;
            int x1 = iconRect.center().x() + static_cast<int>(15 * cos(angle));
            int y1 = iconRect.center().y() + static_cast<int>(15 * sin(angle));
            painter.drawLine(iconRect.center(), QPoint(x1, y1));
        }
        // Draw outer warning triangles
        painter.setBrush(QColor(255, 0, 0, 180));
        for (int i = 0; i < 3; i++) {
            float angle = (i * 120.0f + 60.0f) * static_cast<float>(M_PI) / 180.0f;
            QPolygon triangle;
            int x = iconRect.center().x() + static_cast<int>(20 * cos(angle));
            int y = iconRect.center().y() + static_cast<int>(20 * sin(angle));
            triangle << QPoint(x, y)
                     << QPoint(x - 3, y + 5)
                     << QPoint(x + 3, y + 5);
            painter.drawPolygon(triangle);
        }
        break;
    case SHOVEL:
        // Draw shovel
        painter.setBrush(QColor(139, 69, 19)); // Brown handle
        painter.drawRect(iconRect.center().x() - 3, iconRect.center().y() - 15, 6, 30);
        painter.setBrush(QColor(192, 192, 192)); // Silver blade
        painter.drawRect(iconRect.center().x() - 10, iconRect.center().y() - 20, 20, 10);
        break;
    default:
        break;
    }

    // Cost display
    painter.setBrush(QColor(255, 255, 200));
    painter.drawRect(10, 75, 60, 20);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(QRect(10, 75, 60, 20), Qt::AlignCenter, QString::number(m_cost));

    // Cooldown overlay
    if (m_cooldownTimer > 0.0f) {
        float progress = m_cooldownTimer / m_cooldownDuration;
        int overlayHeight = static_cast<int>((height() - 10) * progress);
        
        painter.setBrush(QColor(0, 0, 0, 150));
        painter.setPen(Qt::NoPen);
        painter.drawRect(5, 5, width() - 10, overlayHeight);
        
        // Cooldown text - use ceil to ensure we show correct remaining time
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 14, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, QString::number(static_cast<int>(ceil(m_cooldownTimer))));
    }

    // Disabled overlay
    if (!m_enabled) {
        painter.setBrush(QColor(0, 0, 0, 100));
        painter.setPen(Qt::NoPen);
        painter.drawRect(5, 5, width() - 10, height() - 10);
    }
}

void PlantCard::mousePressEvent(QMouseEvent* /*event*/)
{
    if (m_enabled && m_cooldownTimer <= 0.0f) {
        emit clicked(m_plantType);
    }
}
