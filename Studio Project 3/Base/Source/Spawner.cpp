#include "Spawner.h"

Spawner::Spawner()
{

}

Spawner::~Spawner()
{

}

bool Spawner::getIsSpawn()
{
    return this->m_IsSpawn;
}

void Spawner::setIsSpawn(bool m_IsSpawn)
{
    this->m_IsSpawn = m_IsSpawn;
}

void Spawner::CheckCount(int CurrentCount, int MaxCount)
{
    if (CurrentCount < MaxCount)
        this->m_IsSpawn = true;
    else
        this->m_IsSpawn = false;
}