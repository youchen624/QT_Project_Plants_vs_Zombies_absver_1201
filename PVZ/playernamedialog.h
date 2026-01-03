#ifndef PLAYERNAMEWIDGET_H
#define PLAYERNAMEWIDGET_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class PlayerNameDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PlayerNameDialog(QWidget *parent = nullptr);
    QString getPlayerName() const;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QLineEdit *m_nameEdit;
    QPushButton *m_okButton;
    QPushButton *m_skipButton;
    QLabel *m_titleLabel;
};

#endif // PLAYERNAMEWIDGET_H
