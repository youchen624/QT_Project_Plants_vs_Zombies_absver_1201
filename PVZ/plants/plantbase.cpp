#include "plantbase.h"

Plant::Plant(QObject *parent)
    : Entity(parent)
    , m_cost(50)
    , m_cooldown(5.0f)
{
}

Plant::~Plant()
{
}
