#include "mainmenu.h"
#include <QPainter>
#include <QFont>

MainMenu::MainMenu(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(800, 600);

    // Create title label
    m_titleLabel = new QLabel("Plants vs Zombies", this);
    m_titleLabel->setFont(QFont("Arial", 36, QFont::Bold));
    m_titleLabel->setStyleSheet("QLabel { color: white; }");
    m_titleLabel->setAlignment(Qt::AlignCenter);

    // Create buttons
    m_selectLevelButton = new QPushButton("遊玩", this);
    m_exitButton = new QPushButton("離開", this);

    // Style buttons
    QString buttonStyle = 
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   border: none;"
        "   color: white;"
        "   padding: 15px 32px;"
        "   text-align: center;"
        "   font-size: 16px;"
        "   margin: 4px 2px;"
        "   border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45a049;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #3d8b40;"
        "}";

    m_selectLevelButton->setStyleSheet(buttonStyle);
    m_exitButton->setStyleSheet(buttonStyle);

    m_selectLevelButton->setMinimumSize(250, 50);
    m_exitButton->setMinimumSize(250, 50);

    // Layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(m_titleLabel, 0, Qt::AlignCenter);
    layout->addSpacing(50);
    layout->addWidget(m_selectLevelButton, 0, Qt::AlignCenter);
    layout->addWidget(m_exitButton, 0, Qt::AlignCenter);
    layout->addStretch();

    // Connect signals
    connect(m_selectLevelButton, &QPushButton::clicked, this, &MainMenu::selectLevel);
    connect(m_exitButton, &QPushButton::clicked, this, &MainMenu::exitGame);
}

MainMenu::~MainMenu()
{
}

void MainMenu::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);
    
    // Draw gradient background
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, QColor(34, 139, 34));  // Forest green
    gradient.setColorAt(1, QColor(0, 100, 0));     // Dark green
    painter.fillRect(rect(), gradient);
}
