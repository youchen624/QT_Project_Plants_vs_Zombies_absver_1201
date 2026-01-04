#include "playernamedialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QLinearGradient>

PlayerNameDialog::PlayerNameDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("輸入玩家名稱");
    setFixedSize(400, 200);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Title
    m_titleLabel = new QLabel("請輸入您的名稱以記錄成績:", this);
    m_titleLabel->setFont(QFont("Arial", 14, QFont::Bold));
    m_titleLabel->setStyleSheet("QLabel { color: white; }");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_titleLabel);
    
    mainLayout->addSpacing(20);
    
    // Name input
    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText("輸入玩家名稱...");
    m_nameEdit->setMaxLength(20);
    m_nameEdit->setStyleSheet(
        "QLineEdit {"
        "   background-color: white;"
        "   border: 2px solid #4CAF50;"
        "   border-radius: 5px;"
        "   padding: 8px;"
        "   font-size: 14px;"
        "}");
    mainLayout->addWidget(m_nameEdit);
    
    mainLayout->addSpacing(20);
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    m_okButton = new QPushButton("確定", this);
    m_skipButton = new QPushButton("跳過", this);
    
    QString buttonStyle = 
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   border: none;"
        "   color: white;"
        "   padding: 10px 20px;"
        "   text-align: center;"
        "   font-size: 14px;"
        "   margin: 4px 2px;"
        "   border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45a049;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #3d8b40;"
        "}";
    
    m_okButton->setStyleSheet(buttonStyle);
    m_skipButton->setStyleSheet(buttonStyle);
    
    connect(m_okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_skipButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_nameEdit, &QLineEdit::returnPressed, this, &QDialog::accept);
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_okButton);
    buttonLayout->addWidget(m_skipButton);
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);
    
    m_nameEdit->setFocus();
}

QString PlayerNameDialog::getPlayerName() const
{
    QString name = m_nameEdit->text().trimmed();
    return name.isEmpty() ? "Anonymous" : name;
}

void PlayerNameDialog::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);
    
    // Draw gradient background
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, QColor(34, 139, 34));  // Forest green
    gradient.setColorAt(1, QColor(0, 100, 0));     // Dark green
    painter.fillRect(rect(), gradient);
}
