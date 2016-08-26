#include "GiantSquidTentacle.h"

GiantSquidTentacle::GiantSquidTentacle()
    : m_TentaclePos(Vector3(0, 0, 0))
    , m_TentacleInitialRotate(0.f)
    , m_TentacleAnimateRotate(0.f)
    , m_TentacleAnimateRotate2(0.f)
    , m_health(0)
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

float GiantSquidTentacle::getTentacleInitialRotate()
{
    return this->m_TentacleInitialRotate;
}

void GiantSquidTentacle::setTentacleInitialRotate(float m_TentacleInitialRotate)
{
    this->m_TentacleInitialRotate = m_TentacleInitialRotate;
}

float GiantSquidTentacle::getTentacleAnimateRotate()
{
    return this->m_TentacleAnimateRotate;
}

void GiantSquidTentacle::setTentacleAnimateRotate(float m_TentacleAnimateRotate)
{
    this->m_TentacleAnimateRotate = m_TentacleAnimateRotate;
}

float GiantSquidTentacle::getTentacleAnimateRotate2()
{
    return this->m_TentacleAnimateRotate2;
}

void GiantSquidTentacle::setTentacleAnimateRotate2(float m_TentacleAnimateRotate2)
{
    this->m_TentacleAnimateRotate2 = m_TentacleAnimateRotate2;
}

int GiantSquidTentacle::getHealth()
{
    return this->m_health;
}

void GiantSquidTentacle::setHealth(int m_health)
{
    this->m_health = m_health;
}