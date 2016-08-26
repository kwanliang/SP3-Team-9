#include "Boss.h"

Boss::Boss()
    : m_health(0)
    , bossType(GIANTSQUID)
    , GameObject()
{

}

Boss::~Boss()
{

}

int Boss::getHealth()
{
    return this->m_health;
}

void Boss::setHealth(int m_health)
{
    this->m_health = m_health;
}