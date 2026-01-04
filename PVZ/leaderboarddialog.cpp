#include "leaderboarddialog.h"
#include "networkleaderboardmanager.h"
#include <QHeaderView>
#include <QPainter>
#include <QLinearGradient>
#include <QDebug>

LeaderboardDialog::LeaderboardDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("排行榜 - Leaderboard");
    setMinimumSize(900, 700);
    setupUI();
    
    // Connect network signals
    connect(&NetworkLeaderboardManager::instance(), &NetworkLeaderboardManager::leaderboardReceived,
            this, &LeaderboardDialog::onOnlineLeaderboardReceived);
    connect(&NetworkLeaderboardManager::instance(), &NetworkLeaderboardManager::networkError,
            this, &LeaderboardDialog::onNetworkError);
    
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
    
    // Online Leaderboard Section
    QLabel *onlineTitle = new QLabel("🌐 線上排行榜 (Online Leaderboard - Top 10)", this);
    onlineTitle->setStyleSheet("QLabel { color: white; font-size: 16px; font-weight: bold; }");
    mainLayout->addWidget(onlineTitle);
    
    m_onlineDisplay = new QTextEdit(this);
    m_onlineDisplay->setReadOnly(true);
    m_onlineDisplay->setMaximumHeight(150);
    m_onlineDisplay->setStyleSheet(
        "QTextEdit {"
        "   background-color: rgba(255, 255, 255, 200);"
        "   border: 2px solid #2196F3;"
        "   border-radius: 5px;"
        "   font-family: 'Courier New', monospace;"
        "   font-size: 12px;"
        "   padding: 5px;"
        "}");
    m_onlineDisplay->setText("點擊「載入線上排行」按鈕查看全球排行榜\nClick 'Load Online Leaderboard' button to view global rankings");
    mainLayout->addWidget(m_onlineDisplay);
    
    m_onlineStatusLabel = new QLabel("", this);
    m_onlineStatusLabel->setStyleSheet("QLabel { color: white; font-size: 12px; }");
    mainLayout->addWidget(m_onlineStatusLabel);
    
    // Separator
    QLabel *localTitle = new QLabel("📋 本地排行榜 (Local Leaderboard)", this);
    localTitle->setStyleSheet("QLabel { color: white; font-size: 16px; font-weight: bold; margin-top: 10px; }");
    mainLayout->addWidget(localTitle);
    
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
    
    m_onlineButton = new QPushButton("🌐 載入線上排行", this);
    m_refreshButton = new QPushButton("🔄 重新整理本地", this);
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
    
    QString onlineButtonStyle = 
        "QPushButton {"
        "   background-color: #2196F3;"
        "   border: none;"
        "   color: white;"
        "   padding: 10px 20px;"
        "   text-align: center;"
        "   font-size: 14px;"
        "   margin: 4px 2px;"
        "   border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #0b7dda;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #0960a5;"
        "}";
    
    m_onlineButton->setStyleSheet(onlineButtonStyle);
    m_refreshButton->setStyleSheet(buttonStyle);
    m_closeButton->setStyleSheet(buttonStyle);
    
    connect(m_onlineButton, &QPushButton::clicked, this, &LeaderboardDialog::loadOnlineLeaderboard);
    connect(m_refreshButton, &QPushButton::clicked, this, &LeaderboardDialog::refreshLeaderboard);
    connect(m_closeButton, &QPushButton::clicked, this, &QDialog::accept);
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_onlineButton);
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

void LeaderboardDialog::loadOnlineLeaderboard()
{
    m_onlineStatusLabel->setText("🔄 正在載入線上排行榜... (Loading online leaderboard...)");
    m_onlineStatusLabel->setStyleSheet("QLabel { color: yellow; font-size: 12px; }");
    m_onlineDisplay->setText("載入中... (Loading...)");
    
    qDebug() << "Fetching online leaderboard...";
    NetworkLeaderboardManager::instance().fetchLeaderboard(0); // Fetch all levels, top scores
}

void LeaderboardDialog::onOnlineLeaderboardReceived(const QVector<PlayerScore> &scores)
{
    qDebug() << "Received" << scores.size() << "online scores";
    
    if (scores.isEmpty()) {
        m_onlineStatusLabel->setText("ℹ️ 線上排行榜暫無資料 (No online data available)");
        m_onlineStatusLabel->setStyleSheet("QLabel { color: #FFA500; font-size: 12px; }");
        m_onlineDisplay->setText("目前沒有線上排行資料\nNo online leaderboard data available yet");
        return;
    }
    
    // Take only top 10
    int displayCount = qMin(10, scores.size());
    QVector<PlayerScore> top10;
    for (int i = 0; i < displayCount; i++) {
        top10.append(scores[i]);
    }
    
    updateOnlineDisplay(top10);
    m_onlineStatusLabel->setText(QString("✅ 已載入 %1 筆線上記錄 (Loaded %1 online records)").arg(displayCount));
    m_onlineStatusLabel->setStyleSheet("QLabel { color: #4CAF50; font-size: 12px; }");
}

void LeaderboardDialog::onNetworkError(const QString &error)
{
    qDebug() << "Network error:" << error;
    m_onlineStatusLabel->setText("❌ 網路錯誤: " + error + " (Network error)");
    m_onlineStatusLabel->setStyleSheet("QLabel { color: red; font-size: 12px; }");
    m_onlineDisplay->setText("無法載入線上排行榜\n" + error + "\n\nUnable to load online leaderboard\n" + error);
}

void LeaderboardDialog::updateOnlineDisplay(const QVector<PlayerScore> &scores)
{
    QString displayText;
    displayText += "═══════════════════════════════════════════════════════════════\n";
    displayText += "  🌐 全球排行榜前 " + QString::number(scores.size()) + " 名 (Global Top " + QString::number(scores.size()) + ")\n";
    displayText += "═══════════════════════════════════════════════════════════════\n\n";
    
    for (int i = 0; i < scores.size(); i++) {
        const PlayerScore &score = scores[i];
        
        QString medal = "";
        if (i == 0) medal = "🥇 ";
        else if (i == 1) medal = "🥈 ";
        else if (i == 2) medal = "🥉 ";
        else medal = QString("   %1. ").arg(i + 1, 2);
        
        QString levelText = (score.levelId == 999) ? "無盡" : QString("Lv%1").arg(score.levelId);
        QString resultIcon = score.isWin ? "✓" : "✗";
        
        displayText += QString("%1%2 | %3 | %4 | 波:%5 殭屍:%6 分數:%7\n")
            .arg(medal)
            .arg(score.playerName, -15)
            .arg(levelText, 4)
            .arg(resultIcon)
            .arg(score.wavesSurvived, 2)
            .arg(score.zombiesKilled, 3)
            .arg(score.calculateScore(), 5);
    }
    
    displayText += "\n═══════════════════════════════════════════════════════════════\n";
    
    m_onlineDisplay->setText(displayText);
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
