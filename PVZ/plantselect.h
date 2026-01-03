#ifndef PLANTSELECT_H
#define PLANTSELECT_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QVector>
#include <QSet>
#include "plantcard.h"

class PlantSelect : public QWidget
{
    Q_OBJECT
public:
    explicit PlantSelect(QWidget *parent = nullptr);
    ~PlantSelect();

    QVector<PlantCard::PlantType> getSelectedPlants() const { return m_selectedPlants; }

signals:
    void plantsSelected(QVector<PlantCard::PlantType> plants);
    void backToMenu();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onPlantCardClicked(int index);
    void onStartGameClicked();

private:
    void setupUI();
    
    QLabel *m_titleLabel;
    QLabel *m_instructionLabel;
    QPushButton *m_backButton;
    QPushButton *m_startButton;
    QScrollArea *m_scrollArea;
    QWidget *m_scrollContent;
    QVector<QPushButton*> m_plantButtons;
    QVector<PlantCard::PlantType> m_availablePlants;
    QVector<PlantCard::PlantType> m_selectedPlants;
    QSet<int> m_selectedIndices;
    
    static const int MAX_PLANTS = 10;
};

#endif // PLANTSELECT_H
