#ifndef LEADERBOARDDIALOG_H
#define LEADERBOARDDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include "leaderboardmanager.h"

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
    void loadOnlineLeaderboard();
    void onOnlineLeaderboardReceived(const QVector<PlayerScore> &scores);
    void onNetworkError(const QString &error);

private:
    void setupUI();
    void updateTable(const QVector<PlayerScore> &scores);
    void updateOnlineDisplay(const QVector<PlayerScore> &scores);
    QString formatTime(qint64 seconds) const;

    QTableWidget *m_tableWidget;
    QPushButton *m_closeButton;
    QPushButton *m_refreshButton;
    QPushButton *m_onlineButton;
    QComboBox *m_filterCombo;
    QLabel *m_titleLabel;
    QLabel *m_onlineStatusLabel;
    QTextEdit *m_onlineDisplay;
};

#endif // LEADERBOARDDIALOG_H
