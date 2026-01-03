#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class PauseMenu : public QWidget
{
    Q_OBJECT
public:
    explicit PauseMenu(QWidget *parent = nullptr);
    ~PauseMenu();

signals:
    void continueGame();
    void showOptions();
    void exitToMenu();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPushButton *m_continueButton;
    QPushButton *m_optionsButton;
    QPushButton *m_exitButton;
    QLabel *m_titleLabel;
};

#endif // PAUSEMENU_H
