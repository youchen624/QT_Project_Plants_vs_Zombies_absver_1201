#include "levelselect.h"
#include <QPainter>
#include <QFont>

LevelSelect::LevelSelect(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(1100, 600);
    setupUI();
}

LevelSelect::~LevelSelect()
{
}

void LevelSelect::setupUI()
{
    // Create title label
    m_titleLabel = new QLabel("選擇關卡", this);
    m_titleLabel->setFont(QFont("Arial", 32, QFont::Bold));
    m_titleLabel->setStyleSheet("QLabel { color: white; }");
    m_titleLabel->setAlignment(Qt::AlignCenter);

    // Create level data
    struct LevelInfo {
        int id;
        QString name;
        QString difficulty;
    };
    
    QVector<LevelInfo> levels = {
        {1, "Level 1", "小屁孩⭐"},
        {2, "Level 2", "國小⭐⭐"},
        {3, "Level 3", "國中⭐⭐⭐"},
        {4, "Level 4", "高中⭐⭐⭐⭐"},
        {5, "Level 5", "虎科⭐⭐⭐⭐⭐"},
        {999, "Endless\n無盡模式(虎科資工系)", "♾️"}
    };

    // Create level buttons in a grid
    QGridLayout *levelGrid = new QGridLayout();
    levelGrid->setSpacing(20);
    
    for (int i = 0; i < levels.size(); ++i) {
        QPushButton *btn = createLevelButton(levels[i].id, 
                                               levels[i].name, 
                                               levels[i].difficulty);
        m_levelButtons.append(btn);
        
        // Arrange in 2 rows
        int row = i / 3;
        int col = i % 3;
        levelGrid->addWidget(btn, row, col, Qt::AlignCenter);
        
        // Connect button to signal
        connect(btn, &QPushButton::clicked, this, [this, id = levels[i].id]() {
            emit levelSelected(id);
        });
    }

    // Create back button
    m_backButton = new QPushButton("返回選單", this);
    m_backButton->setMinimumSize(200, 50);
    
    QString backButtonStyle = 
        "QPushButton {"
        "   background-color: #f44336;"
        "   border: none;"
        "   color: white;"
        "   padding: 15px 32px;"
        "   text-align: center;"
        "   font-size: 16px;"
        "   border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #da190b;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #c1150a;"
        "}";
    m_backButton->setStyleSheet(backButtonStyle);
    
    connect(m_backButton, &QPushButton::clicked, this, &LevelSelect::backToMenu);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addSpacing(30);
    mainLayout->addWidget(m_titleLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(30);
    mainLayout->addLayout(levelGrid);
    mainLayout->addStretch();
    mainLayout->addWidget(m_backButton, 0, Qt::AlignCenter);
    mainLayout->addSpacing(20);
}

QPushButton* LevelSelect::createLevelButton(int levelId, const QString &levelName, 
                                              const QString &difficulty)
{
    QPushButton *btn = new QPushButton(this);
    btn->setMinimumSize(300, 120);
    btn->setMaximumSize(300, 120);
    
    // Create button text with name and difficulty
    // QPushButton doesn't support setTextFormat, use plain text with newlines
    QString text = QString("%1\n%2")
                       .arg(levelName)
                       .arg(difficulty);
    btn->setText(text);
    
    // Style based on level difficulty
    QColor baseColor;
    if (levelId == 1) {
        baseColor = QColor(76, 175, 80);  // Green - Easy
    } else if (levelId == 2) {
        baseColor = QColor(33, 150, 243);  // Blue - Medium
    } else if (levelId == 3) {
        baseColor = QColor(255, 152, 0);   // Orange - Hard
    } else if (levelId == 4) {
        baseColor = QColor(156, 39, 176);  // Purple - Very Hard
    } else {
        baseColor = QColor(244, 67, 54);   // Red - Extreme
    }
    
    QString buttonStyle = QString(
        "QPushButton {"
        "   background-color: rgb(%1, %2, %3);"
        "   border: 3px solid white;"
        "   color: white;"
        "   padding: 10px;"
        "   border-radius: 12px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgb(%4, %5, %6);"
        "   border: 3px solid yellow;"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgb(%7, %8, %9);"
        "}")
        .arg(baseColor.red()).arg(baseColor.green()).arg(baseColor.blue())
        .arg(qMax(0, baseColor.red() - 20)).arg(qMax(0, baseColor.green() - 20)).arg(qMax(0, baseColor.blue() - 20))
        .arg(qMax(0, baseColor.red() - 40)).arg(qMax(0, baseColor.green() - 40)).arg(qMax(0, baseColor.blue() - 40));
    
    btn->setStyleSheet(buttonStyle);
    
    return btn;
}

void LevelSelect::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);
    
    // Draw gradient background
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, QColor(25, 118, 210));  // Blue
    gradient.setColorAt(1, QColor(13, 71, 161));   // Dark blue
    painter.fillRect(rect(), gradient);
}
