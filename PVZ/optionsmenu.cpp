#include "optionsmenu.h"
#include <QPainter>
#include <QFont>

OptionsMenu::OptionsMenu(QWidget *parent)
    : QWidget(parent)
    , m_autoCollect(false)
    , m_gameSpeed(1.0f)
{
    setupUI();
}

OptionsMenu::~OptionsMenu()
{
}

void OptionsMenu::setupUI()
{
    setFixedSize(400, 350);
    
    // Title
    m_titleLabel = new QLabel("選項", this);
    m_titleLabel->setFont(QFont("Arial", 24, QFont::Bold));
    m_titleLabel->setStyleSheet("QLabel { color: white; }");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    
    // Auto-collect checkbox
    m_autoCollectCheckbox = new QCheckBox("自動收集陽光", this);
    m_autoCollectCheckbox->setStyleSheet(
        "QCheckBox {"
        "  color: white;"
        "  font-size: 16px;"
        "  spacing: 10px;"
        "}"
        "QCheckBox::indicator {"
        "  width: 20px;"
        "  height: 20px;"
        "}"
    );
    connect(m_autoCollectCheckbox, &QCheckBox::toggled, this, &OptionsMenu::onAutoCollectToggled);
    
    // Game speed combo
    QLabel *speedLabel = new QLabel("倍速:", this);
    speedLabel->setStyleSheet("QLabel { color: white; font-size: 16px; }");
    
    m_gameSpeedCombo = new QComboBox(this);
    m_gameSpeedCombo->addItem("1x (正常)", 1.0f);
    m_gameSpeedCombo->addItem("3x (加倍)", 3.0f);
    m_gameSpeedCombo->addItem("5x (加加倍)", 5.0f);
    m_gameSpeedCombo->setStyleSheet(
        "QComboBox {"
        "  background-color: #4CAF50;"
        "  color: white;"
        "  font-size: 16px;"
        "  padding: 5px;"
        "  border-radius: 5px;"
        "}"
        "QComboBox::drop-down {"
        "  border: none;"
        "}"
        "QComboBox QAbstractItemView {"
        "  background-color: #4CAF50;"
        "  color: white;"
        "  selection-background-color: #45a049;"
        "}"
    );
    connect(m_gameSpeedCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &OptionsMenu::onGameSpeedChanged);
    
    QHBoxLayout *speedLayout = new QHBoxLayout();
    speedLayout->addWidget(speedLabel);
    speedLayout->addWidget(m_gameSpeedCombo);
    
    // Close button
    m_closeButton = new QPushButton("關閉", this);
    m_closeButton->setMinimumSize(150, 40);
    m_closeButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #f44336;"
        "  color: white;"
        "  font-size: 16px;"
        "  font-weight: bold;"
        "  border-radius: 8px;"
        "  padding: 10px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #da190b;"
        "}"
    );
    connect(m_closeButton, &QPushButton::clicked, this, &OptionsMenu::onCloseClicked);
    
    // Layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->addStretch();
    mainLayout->addWidget(m_titleLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(m_autoCollectCheckbox, 0, Qt::AlignCenter);
    mainLayout->addLayout(speedLayout);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(m_closeButton, 0, Qt::AlignCenter);
    mainLayout->addStretch();
}

void OptionsMenu::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);
    
    // Semi-transparent dark background
    painter.fillRect(rect(), QColor(0, 0, 0, 220));
    
    // Border
    painter.setPen(QPen(QColor(100, 200, 100), 3));
    painter.drawRect(rect().adjusted(1, 1, -1, -1));
}

void OptionsMenu::onAutoCollectToggled(bool checked)
{
    m_autoCollect = checked;
    emit autoCollectChanged(checked);
}

void OptionsMenu::onGameSpeedChanged(int index)
{
    m_gameSpeed = m_gameSpeedCombo->itemData(index).toFloat();
    emit gameSpeedChanged(m_gameSpeed);
}

void OptionsMenu::onCloseClicked()
{
    emit closeMenu();
}

void OptionsMenu::setAutoCollectEnabled(bool enabled)
{
    m_autoCollect = enabled;
    m_autoCollectCheckbox->setChecked(enabled);
}

void OptionsMenu::setGameSpeed(float speed)
{
    m_gameSpeed = speed;
    
    // Find and set the correct index in combo box
    for (int i = 0; i < m_gameSpeedCombo->count(); ++i) {
        if (qAbs(m_gameSpeedCombo->itemData(i).toFloat() - speed) < 0.01f) {
            m_gameSpeedCombo->blockSignals(true); // Prevent triggering signal
            m_gameSpeedCombo->setCurrentIndex(i);
            m_gameSpeedCombo->blockSignals(false);
            break;
        }
    }
}
