#ifndef LEADERBOARDDIALOG_H
#define LEADERBOARDDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLabel>
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

private:
    void setupUI();
    void updateTable(const QVector<PlayerScore> &scores);
    QString formatTime(qint64 seconds) const;

    QTableWidget *m_tableWidget;
    QPushButton *m_closeButton;
    QPushButton *m_refreshButton;
    QComboBox *m_filterCombo;
    QLabel *m_titleLabel;
};

#endif // LEADERBOARDDIALOG_H
