#include "SeaCreature.h"

SeaCreature::SeaCreature()
    : m_health(100)
    , seaType(MINNOW)
    , m_DebounceTimer(0)
    , m_canVacuum(false)
	, isstunned(false)
	, isVacuum(false)
    , GameObject()
{

}

SeaCreature::~SeaCreature()
{

}

int SeaCreature::getHealth()
{
    return this->m_health;
}


float SeaCreature::getDebounceTimer()
{
    return this->m_DebounceTimer;
}

bool SeaCreature::getCanVacuum()
{
    return this->m_canVacuum;
}

bool SeaCreature::getisVacuum()
{
	return this->isVacuum;
}

void SeaCreature::setDebounceTimer(float timer)
{
    this->m_DebounceTimer = timer;
}

void SeaCreature::setHealth(int m_health)
{
    this->m_health = m_health;
}

void SeaCreature::setCanVacuum(bool m_canVacuum)
{
    this->m_canVacuum = m_canVacuum;
}

void SeaCreature::setisVacuum(bool isVacuum)
{
	this->isVacuum = isVacuum;
}


Vector3 SeaCreature::cohesion(Vector3 playerpos, Vector3 playertarget)
{
    if (!playerpos.IsZero() || !playertarget.IsZero())
    {
		Vector3 tv(0, 0, 0);
		if (!(playertarget * -1).IsZero())
        tv = (playertarget * -1).Normalized() * 30;
        Vector3 behindPos = playerpos + tv;
		if (!(behindPos - this->pos).IsZero())
        behindPos = (behindPos - this->pos).Normalized();

        return behindPos;
    }
    return Vector3(0, 0, 0);
}

Vector3 SeaCreature::seperation(Vector3 repelVector)
{
    float tempIntensity = 0.f;

    if (!repelVector.IsZero())
    {
        tempIntensity = (repelVector.LengthSquared() / g_distFromSeperationIntensity);
        repelVector = repelVector.Normalized();
        repelVector = Vector3(repelVector.x / tempIntensity, repelVector.y / tempIntensity, repelVector.z / tempIntensity);

        return repelVector.Normalized();
    }
    return Vector3(0, 0, 0);
}

Vector3 SeaCreature::alignment(Vector3 forceVector)
{
    if (!forceVector.IsZero())
    {
        return forceVector.Normalized();
    }
    return Vector3(0, 0, 0);
}