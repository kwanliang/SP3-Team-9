#include "Spawner.h"

Spawner::Spawner()
{

}

Spawner::~Spawner()
{

}

Vector3 Spawner::getPos()
{
    return this->m_pos;
}

void Spawner::setPos(Vector3 m_pos)
{
    this->m_pos = m_pos;
}

bool Spawner::getIsSpawn()
{
    return this->m_IsSpawn;
}

void Spawner::setIsSpawn(bool m_IsSpawn)
{
    this->m_IsSpawn = m_IsSpawn;
}

Vector3 Spawner::getMinRange()
{
    return this->m_MinRange;
}

void Spawner::setMinRange(Vector3 m_MinRange)
{
    this->m_MinRange = m_MinRange;
}

Vector3 Spawner::getMaxRange()
{
    return this->m_MaxRange;
}

void Spawner::setMaxRange(Vector3 m_MaxRange)
{
    this->m_MaxRange = m_MaxRange;
}

void Spawner::CheckCount(int CurrentCount, int MaxCount)
{
    if (CurrentCount < MaxCount)
        this->m_IsSpawn = true;
    else
        this->m_IsSpawn = false;
}