#ifndef SKELETONTREEPLANT_H
#define SKELETONTREEPLANT_H

#include "plantbase.h"

// Skeleton Tree plant (summons skeletons on upper/lower lanes, not current lane)
class SkeletonTreePlant : public Plant
{
    Q_OBJECT
public:
    explicit SkeletonTreePlant(QObject *parent = nullptr);
    void update(float deltaTime) override;
    void render(QPainter *painter) override;

signals:
    void summonSkeleton(int treeRow); // Emit tree's row to trigger skeleton summoning on adjacent lanes

private:
    float m_summonTimer;
    float m_summonInterval;
};

#endif // SKELETONTREEPLANT_H
