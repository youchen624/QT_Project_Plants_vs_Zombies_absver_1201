#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class OptionsMenu : public QWidget
{
    Q_OBJECT
public:
    explicit OptionsMenu(QWidget *parent = nullptr);
    ~OptionsMenu();

    bool isAutoCollectEnabled() const { return m_autoCollect; }
    float getGameSpeed() const { return m_gameSpeed; }
    
    void setAutoCollectEnabled(bool enabled);
    void setGameSpeed(float speed);

signals:
    void closeMenu();
    void autoCollectChanged(bool enabled);
    void gameSpeedChanged(float speed);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onAutoCollectToggled(bool checked);
    void onGameSpeedChanged(int index);
    void onCloseClicked();

private:
    void setupUI();

    QLabel *m_titleLabel;
    QCheckBox *m_autoCollectCheckbox;
    QComboBox *m_gameSpeedCombo;
    QPushButton *m_closeButton;
    
    bool m_autoCollect;
    float m_gameSpeed;
};

#endif // OPTIONSMENU_H
