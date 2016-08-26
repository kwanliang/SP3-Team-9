#include "GiantSquidTentacle.h"

GiantSquidTentacle::GiantSquidTentacle()
    : m_TentaclePos(Vector3(0, 0, 0))
    , m_scale(Vector3(0, 0, 0))
    , m_health(0)
    , m_active(false)
{

}

GiantSquidTentacle::~GiantSquidTentacle()
{

}

Vector3 GiantSquidTentacle::getTentaclePos()
{
    return this->m_TentaclePos;
}

void GiantSquidTentacle::setTentaclePos(Vector3 m_TentaclePos)
{
    this->m_TentaclePos = m_TentaclePos;
}

Vector3 GiantSquidTentacle::getScale()
{
    return this->m_scale;
}

void GiantSquidTentacle::setScale(Vector3 m_scale)
{
    this->m_scale = m_scale;
}

int GiantSquidTentacle::getHealth()
{
    return this->m_health;
}

void GiantSquidTentacle::setHealth(int m_health)
{
    this->m_health = m_health;
}

//bool GiantSquidTentacle::getActive()
//{
//    return this->m_active;
//}
//
//void GiantSquidTentacle::setActive(bool m_active)
//{
//    this->m_active = m_active;
//}