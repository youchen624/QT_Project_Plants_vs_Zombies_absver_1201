#include "pausemenu.h"
#include <QPainter>
#include <QFont>

PauseMenu::PauseMenu(QWidget *parent)
    : QWidget(parent)
{
    // Set fixed size for the pause menu
    setFixedSize(400, 350);
    
    // Create title label
    m_titleLabel = new QLabel("遊戲暫停", this);
    m_titleLabel->setFont(QFont("Arial", 28, QFont::Bold));
    m_titleLabel->setStyleSheet("QLabel { color: white; }");
    m_titleLabel->setAlignment(Qt::AlignCenter);

    // Create buttons
    m_continueButton = new QPushButton("繼續遊戲", this);
    m_optionsButton = new QPushButton("選項", this);
    m_exitButton = new QPushButton("離開至主選單", this);

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

    m_continueButton->setStyleSheet(buttonStyle);
    m_optionsButton->setStyleSheet(buttonStyle);
    m_exitButton->setStyleSheet(buttonStyle);

    m_continueButton->setMinimumSize(200, 50);
    m_optionsButton->setMinimumSize(200, 50);
    m_exitButton->setMinimumSize(200, 50);

    // Layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(m_titleLabel, 0, Qt::AlignCenter);
    layout->addSpacing(30);
    layout->addWidget(m_continueButton, 0, Qt::AlignCenter);
    layout->addWidget(m_optionsButton, 0, Qt::AlignCenter);
    layout->addWidget(m_exitButton, 0, Qt::AlignCenter);
    layout->addStretch();

    // Connect signals
    connect(m_continueButton, &QPushButton::clicked, this, &PauseMenu::continueGame);
    connect(m_optionsButton, &QPushButton::clicked, this, &PauseMenu::showOptions);
    connect(m_exitButton, &QPushButton::clicked, this, &PauseMenu::exitToMenu);
}

PauseMenu::~PauseMenu()
{
}

void PauseMenu::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);
    
    // Draw semi-transparent background
    painter.fillRect(rect(), QColor(0, 0, 0, 180));
    
    // Draw border
    painter.setPen(QPen(QColor(255, 255, 255, 200), 3));
    painter.drawRect(rect().adjusted(1, 1, -1, -1));
}
