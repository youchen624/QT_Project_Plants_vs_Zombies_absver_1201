#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class MainMenu : public QWidget
{
    Q_OBJECT
public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

signals:
    void selectLevel();
    void showLeaderboard();
    void exitGame();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPushButton *m_selectLevelButton;
    QPushButton *m_leaderboardButton;
    QPushButton *m_exitButton;
    QLabel *m_titleLabel;
};

#endif // MAINMENU_H
