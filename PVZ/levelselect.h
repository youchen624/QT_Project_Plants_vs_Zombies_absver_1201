#ifndef LEVELSELECT_H
#define LEVELSELECT_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QVector>

class LevelSelect : public QWidget
{
    Q_OBJECT
public:
    explicit LevelSelect(QWidget *parent = nullptr);
    ~LevelSelect();

signals:
    void levelSelected(int levelId);
    void backToMenu();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void setupUI();
    QPushButton* createLevelButton(int levelId, const QString &levelName, 
                                     const QString &difficulty);

    QLabel *m_titleLabel;
    QPushButton *m_backButton;
    QVector<QPushButton*> m_levelButtons;
};

#endif // LEVELSELECT_H
