#include "leaderboarddialog.h"
#include <QHeaderView>
#include <QPainter>
#include <QLinearGradient>
#include <QDebug>

LeaderboardDialog::LeaderboardDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("排行榜 - Leaderboard");
    setMinimumSize(900, 600);
    setupUI();
    refreshLeaderboard();
}

LeaderboardDialog::~LeaderboardDialog()
{
}

void LeaderboardDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Title
    m_titleLabel = new QLabel("🏆 遊戲排行榜 🏆", this);
    m_titleLabel->setFont(QFont("Arial", 24, QFont::Bold));
    m_titleLabel->setStyleSheet("QLabel { color: white; }");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_titleLabel);
    
    // Filter combo box
    QHBoxLayout *filterLayout = new QHBoxLayout();
    QLabel *filterLabel = new QLabel("篩選:", this);
    filterLabel->setStyleSheet("QLabel { color: white; font-size: 14px; }");
    filterLayout->addWidget(filterLabel);
    
    m_filterCombo = new QComboBox(this);
    m_filterCombo->addItem("全部關卡", 0);
    m_filterCombo->addItem("關卡 1", 1);
    m_filterCombo->addItem("關卡 2", 2);
    m_filterCombo->addItem("關卡 3", 3);
    m_filterCombo->addItem("關卡 4", 4);
    m_filterCombo->addItem("關卡 5", 5);
    m_filterCombo->addItem("無盡模式", 999);
    m_filterCombo->setStyleSheet(
        "QComboBox {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   border: 2px solid #45a049;"
        "   border-radius: 5px;"
        "   padding: 5px;"
        "   font-size: 14px;"
        "}"
        "QComboBox:hover {"
        "   background-color: #45a049;"
        "}"
        "QComboBox::drop-down {"
        "   border: none;"
        "}"
        "QComboBox QAbstractItemView {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   selection-background-color: #45a049;"
        "}");
    connect(m_filterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &LeaderboardDialog::onFilterChanged);
    filterLayout->addWidget(m_filterCombo);
    filterLayout->addStretch();
    mainLayout->addLayout(filterLayout);
    
    // Table widget
    m_tableWidget = new QTableWidget(this);
    m_tableWidget->setColumnCount(9);
    m_tableWidget->setHorizontalHeaderLabels({
        "排名", "玩家", "關卡", "結果", "波數", "殭屍擊殺", "植物種植", "陽光收集", "遊玩時間"
    });
    
    // Style table
    m_tableWidget->setStyleSheet(
        "QTableWidget {"
        "   background-color: rgba(255, 255, 255, 200);"
        "   gridline-color: #4CAF50;"
        "   font-size: 13px;"
        "   border: 2px solid #4CAF50;"
        "   border-radius: 5px;"
        "}"
        "QTableWidget::item {"
        "   padding: 5px;"
        "}"
        "QTableWidget::item:selected {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "}"
        "QHeaderView::section {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   padding: 8px;"
        "   border: 1px solid #45a049;"
        "   font-weight: bold;"
        "   font-size: 14px;"
        "}");
    
    m_tableWidget->horizontalHeader()->setStretchLastSection(true);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableWidget->verticalHeader()->setVisible(false);
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableWidget->setAlternatingRowColors(true);
    
    mainLayout->addWidget(m_tableWidget);
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    m_refreshButton = new QPushButton("🔄 重新整理", this);
    m_closeButton = new QPushButton("✖ 關閉", this);
    
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
    
    m_refreshButton->setStyleSheet(buttonStyle);
    m_closeButton->setStyleSheet(buttonStyle);
    
    connect(m_refreshButton, &QPushButton::clicked, this, &LeaderboardDialog::refreshLeaderboard);
    connect(m_closeButton, &QPushButton::clicked, this, &QDialog::accept);
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_refreshButton);
    buttonLayout->addWidget(m_closeButton);
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);
}

void LeaderboardDialog::refreshLeaderboard()
{
    LeaderboardManager::instance().loadFromFile();
    onFilterChanged(m_filterCombo->currentIndex());
}

void LeaderboardDialog::onFilterChanged(int /*index*/)
{
    int levelId = m_filterCombo->currentData().toInt();
    QVector<PlayerScore> scores;
    
    if (levelId == 0) {
        // All levels
        scores = LeaderboardManager::instance().getTopScores(50);
    } else {
        // Specific level
        scores = LeaderboardManager::instance().getScoresByLevel(levelId, 50);
    }
    
    updateTable(scores);
}

void LeaderboardDialog::updateTable(const QVector<PlayerScore> &scores)
{
    m_tableWidget->setRowCount(scores.size());
    
    for (int i = 0; i < scores.size(); i++) {
        const PlayerScore &score = scores[i];
        
        // Rank
        QTableWidgetItem *rankItem = new QTableWidgetItem(QString::number(i + 1));
        rankItem->setTextAlignment(Qt::AlignCenter);
        if (i == 0) rankItem->setBackground(QColor(255, 215, 0)); // Gold
        else if (i == 1) rankItem->setBackground(QColor(192, 192, 192)); // Silver
        else if (i == 2) rankItem->setBackground(QColor(205, 127, 50)); // Bronze
        m_tableWidget->setItem(i, 0, rankItem);
        
        // Player name
        QTableWidgetItem *nameItem = new QTableWidgetItem(score.playerName);
        nameItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        m_tableWidget->setItem(i, 1, nameItem);
        
        // Level
        QString levelText;
        if (score.levelId == 999) {
            levelText = "無盡";
        } else {
            levelText = QString::number(score.levelId);
        }
        QTableWidgetItem *levelItem = new QTableWidgetItem(levelText);
        levelItem->setTextAlignment(Qt::AlignCenter);
        m_tableWidget->setItem(i, 2, levelItem);
        
        // Result
        QString resultText = score.isWin ? "✔ 勝利" : "✖ 失敗";
        QTableWidgetItem *resultItem = new QTableWidgetItem(resultText);
        resultItem->setTextAlignment(Qt::AlignCenter);
        if (score.isWin) {
            resultItem->setForeground(QColor(0, 128, 0));
        } else {
            resultItem->setForeground(QColor(128, 0, 0));
        }
        m_tableWidget->setItem(i, 3, resultItem);
        
        // Waves survived
        QTableWidgetItem *wavesItem = new QTableWidgetItem(QString::number(score.wavesSurvived));
        wavesItem->setTextAlignment(Qt::AlignCenter);
        m_tableWidget->setItem(i, 4, wavesItem);
        
        // Zombies killed
        QTableWidgetItem *zombiesItem = new QTableWidgetItem(QString::number(score.zombiesKilled));
        zombiesItem->setTextAlignment(Qt::AlignCenter);
        m_tableWidget->setItem(i, 5, zombiesItem);
        
        // Plants placed
        QTableWidgetItem *plantsItem = new QTableWidgetItem(QString::number(score.plantsPlaced));
        plantsItem->setTextAlignment(Qt::AlignCenter);
        m_tableWidget->setItem(i, 6, plantsItem);
        
        // Sun collected
        QTableWidgetItem *sunItem = new QTableWidgetItem(QString::number(score.totalSunCollected));
        sunItem->setTextAlignment(Qt::AlignCenter);
        m_tableWidget->setItem(i, 7, sunItem);
        
        // Play time
        QTableWidgetItem *timeItem = new QTableWidgetItem(formatTime(score.playTimeSeconds));
        timeItem->setTextAlignment(Qt::AlignCenter);
        m_tableWidget->setItem(i, 8, timeItem);
    }
}

QString LeaderboardDialog::formatTime(qint64 seconds) const
{
    int mins = seconds / 60;
    int secs = seconds % 60;
    return QString("%1:%2").arg(mins).arg(secs, 2, 10, QChar('0'));
}

void LeaderboardDialog::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);
    
    // Draw gradient background
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, QColor(34, 139, 34));  // Forest green
    gradient.setColorAt(1, QColor(0, 100, 0));     // Dark green
    painter.fillRect(rect(), gradient);
}
