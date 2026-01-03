#include "plantselect.h"
#include <QPainter>
#include <QFont>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

PlantSelect::PlantSelect(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(1100, 600);
    
    // Initialize available plants
    m_availablePlants = {
        PlantCard::SUNFLOWER,
        PlantCard::SHOOTER,
        PlantCard::WALL,
        PlantCard::ICESHOOTER,
        PlantCard::REPEATER,
        PlantCard::NUCLEARBOMB,
        PlantCard::SKELETONTREE,
        PlantCard::GROUNDSPIKE,
        PlantCard::CHILIPEPPER,
        PlantCard::MINE,
        PlantCard::TWINSUNFLOWER,
        PlantCard::CATGRASS,
        PlantCard::SUPERGATLINGPEA,
        PlantCard::SUPERNUCLEARBOMB
    };
    
    setupUI();
}

PlantSelect::~PlantSelect()
{
}

void PlantSelect::setupUI()
{
    // Create title label
    m_titleLabel = new QLabel("選擇植物", this);
    m_titleLabel->setFont(QFont("Arial", 32, QFont::Bold));
    m_titleLabel->setStyleSheet("QLabel { color: white; }");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setGeometry(0, 20, 1100, 50);

    // Create instruction label
    m_instructionLabel = new QLabel(QString("最多攜帶 %1 種植物").arg(MAX_PLANTS), this);
    m_instructionLabel->setFont(QFont("Arial", 16));
    m_instructionLabel->setStyleSheet("QLabel { color: yellow; }");
    m_instructionLabel->setAlignment(Qt::AlignCenter);
    m_instructionLabel->setGeometry(0, 75, 1100, 30);

    // Create scroll area for plant buttons
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setGeometry(50, 120, 1000, 360);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setWidgetResizable(false);
    m_scrollArea->setStyleSheet(
        "QScrollArea {"
        "  background-color: rgba(0, 0, 0, 50);"
        "  border: 2px solid #4CAF50;"
        "  border-radius: 10px;"
        "}"
        "QScrollBar:vertical {"
        "  background: #2E7D32;"
        "  width: 15px;"
        "  margin: 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "  background: #4CAF50;"
        "  min-height: 20px;"
        "  border-radius: 7px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "  background: #66BB6A;"
        "}"
    );

    // Create container widget for plant buttons
    m_scrollContent = new QWidget();
    int buttonWidth = 120;
    int buttonHeight = 150;
    int spacing = 20;
    int cols = 5; // Changed from 3 to 5 for more plants per row
    int rows = (m_availablePlants.size() + cols - 1) / cols; // Calculate needed rows
    int contentWidth = cols * (buttonWidth + spacing) + spacing;
    int contentHeight = rows * (buttonHeight + spacing) + spacing;
    m_scrollContent->setMinimumSize(contentWidth, contentHeight);
    
    // Create plant selection buttons in scroll content
    for (int i = 0; i < m_availablePlants.size(); ++i) {
        QPushButton *btn = new QPushButton(m_scrollContent);
        btn->setFixedSize(buttonWidth, buttonHeight);
        
        int col = i % cols;
        int row = i / cols;
        int x = spacing + col * (buttonWidth + spacing);
        int y = spacing + row * (buttonHeight + spacing);
        btn->move(x, y);
        
        // Style the button based on plant type
        PlantCard::PlantType plantType = m_availablePlants[i];
        QString plantName;
        QString cost;
        QString bgColor;
        
        switch (plantType) {
        case PlantCard::SUNFLOWER:
            plantName = "太陽花";
            cost = "50";
            bgColor = "#FFEB3B";
            break;
        case PlantCard::SHOOTER:
            plantName = "豌豆射手";
            cost = "100";
            bgColor = "#4CAF50";
            break;
        case PlantCard::WALL:
            plantName = "牆壁";
            cost = "50";
            bgColor = "#8B4513";
            break;
        case PlantCard::ICESHOOTER:
            plantName = "寒冰射手";
            cost = "175";
            bgColor = "#64C8FF";
            break;
        case PlantCard::REPEATER:
            plantName = "雙倍豌豆射手";
            cost = "200";
            bgColor = "#009600";
            break;
        case PlantCard::NUCLEARBOMB:
            plantName = "核彈";
            cost = "150";
            bgColor = "#FF4500";
            break;
        case PlantCard::SKELETONTREE:
            plantName = "骷髏樹";
            cost = "350";
            bgColor = "#8B7355";
            break;
        case PlantCard::GROUNDSPIKE:
            plantName = "地刺";
            cost = "100";
            bgColor = "#A9A9A9";
            break;
        case PlantCard::CHILIPEPPER:
            plantName = "火爆爆辣椒";
            cost = "125";
            bgColor = "#DC143C";
            break;
        case PlantCard::MINE:
            plantName = "地雷";
            cost = "25";
            bgColor = "#505050";
            break;
        case PlantCard::TWINSUNFLOWER:
            plantName = "雙倍太陽花";
            cost = "150";
            bgColor = "#FFD700";
            break;
        case PlantCard::CATGRASS:
            plantName = "細菌草";
            cost = "225";
            bgColor = "#C864C8";
            break;
        case PlantCard::SUPERGATLINGPEA:
            plantName = "超級無敵宇宙霹靂\n究極豌豆射手";
            cost = "800";
            bgColor = "#006400";
            break;
        case PlantCard::SUPERNUCLEARBOMB:
            plantName = "\"我C你M\"彈";
            cost = "1500";
            bgColor = "#FF0000";
            break;
        default:
            plantName = "Unknown";
            cost = "0";
            bgColor = "#808080";
        }
        
        btn->setText(plantName + "\n💰 " + cost);
        btn->setStyleSheet(QString(
            "QPushButton {"
            "  background-color: %1;"
            "  border: 3px solid #333;"
            "  border-radius: 10px;"
            "  color: black;"
            "  font-size: 14px;"
            "  font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "  border: 3px solid #00FF00;"
            "}"
            "QPushButton:checked {"
            "  border: 5px solid #00FF00;"
            "  background-color: lightgreen;"
            "}"
        ).arg(bgColor));
        
        btn->setCheckable(true);
        connect(btn, &QPushButton::clicked, this, [this, i]() { onPlantCardClicked(i); });
        
        m_plantButtons.append(btn);
    }
    
    m_scrollArea->setWidget(m_scrollContent);

    // Create back button
    m_backButton = new QPushButton("← Back", this);
    m_backButton->setGeometry(50, 500, 150, 50);
    m_backButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #8B4513;"
        "  color: white;"
        "  font-size: 18px;"
        "  font-weight: bold;"
        "  border: 3px solid #654321;"
        "  border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #A0522D;"
        "}"
    );
    connect(m_backButton, &QPushButton::clicked, this, &PlantSelect::backToMenu);

    // Create start button
    m_startButton = new QPushButton("Start Game →", this);
    m_startButton->setGeometry(900, 500, 150, 50);
    m_startButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #4CAF50;"
        "  color: white;"
        "  font-size: 18px;"
        "  font-weight: bold;"
        "  border: 3px solid #2E7D32;"
        "  border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #66BB6A;"
        "}"
        "QPushButton:disabled {"
        "  background-color: #888;"
        "  color: #444;"
        "}"
    );
    m_startButton->setEnabled(false);
    connect(m_startButton, &QPushButton::clicked, this, &PlantSelect::onStartGameClicked);
}

void PlantSelect::onPlantCardClicked(int index)
{
    if (m_plantButtons[index]->isChecked()) {
        // Try to select this plant
        if (m_selectedIndices.size() < MAX_PLANTS) {
            m_selectedIndices.insert(index);
            m_selectedPlants.append(m_availablePlants[index]);
        } else {
            // Already at max, uncheck it
            m_plantButtons[index]->setChecked(false);
        }
    } else {
        // Deselect this plant
        m_selectedIndices.remove(index);
        // Remove only the specific occurrence at this index
        int plantIndex = m_selectedPlants.indexOf(m_availablePlants[index]);
        if (plantIndex >= 0) {
            m_selectedPlants.removeAt(plantIndex);
        }
    }
    
    // Update instruction label
    m_instructionLabel->setText(QString("Selected: %1/%2 plants").arg(m_selectedIndices.size()).arg(MAX_PLANTS));
    
    // Enable start button if at least one plant is selected
    m_startButton->setEnabled(m_selectedIndices.size() > 0);
}

void PlantSelect::onStartGameClicked()
{
    if (m_selectedPlants.size() > 0) {
        emit plantsSelected(m_selectedPlants);
    }
}

void PlantSelect::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw background gradient
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, QColor(34, 139, 34));    // Forest green
    gradient.setColorAt(1, QColor(85, 107, 47));    // Dark olive green
    painter.fillRect(rect(), gradient);
    
    // Draw decorative grass pattern
    painter.setPen(QPen(QColor(60, 179, 113), 2));
    for (int i = 0; i < 20; i++) {
        int x = (i * 60) % width();
        painter.drawLine(x, height() - 50, x + 20, height());
    }
}
