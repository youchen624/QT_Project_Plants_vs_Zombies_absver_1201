#ifndef LEADERBOARDDIALOG_H
#define LEADERBOARDDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QTabWidget>
#include "leaderboardmanager.h"
#include "networkleaderboardmanager.h"

class LeaderboardDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LeaderboardDialog(QWidget *parent = nullptr);
    ~LeaderboardDialog();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void refreshLeaderboard();
    void onFilterChanged(int index);
    void onTabChanged(int index);
    void onOnlineLeaderboardReceived(const QVector<PlayerScore> &scores);
    void onNetworkError(const QString &error);

private:
    void setupUI();
    void updateTable(const QVector<PlayerScore> &scores);
    QString formatTime(qint64 seconds) const;
    void refreshLocalLeaderboard();
    void refreshOnlineLeaderboard();

    QTabWidget *m_tabWidget;
    QTableWidget *m_tableWidget;
    QPushButton *m_closeButton;
    QPushButton *m_refreshButton;
    QComboBox *m_filterCombo;
    QLabel *m_titleLabel;
    QLabel *m_statusLabel;
    
    int m_currentTabIndex;
    QVector<PlayerScore> m_cachedOnlineScores; // Cache for online leaderboard data
};

#endif // LEADERBOARDDIALOG_H
