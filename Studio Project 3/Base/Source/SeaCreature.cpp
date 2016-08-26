#include "SeaCreature.h"

SeaCreature::SeaCreature()
    : m_health(100)
    , seaType(MINNOW)
    , GameObject(SEACREATURE, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1), false)
	, m_DebounceTimer(0)
{

}

SeaCreature::SeaCreature(int m_health, SEACREATURE_TYPE seaType, OBJECT_TYPE objectType, Vector3 pos, Vector3 vel, Vector3 scale, bool active)
    : m_health(m_health)
    , seaType(seaType)
    , GameObject(objectType, pos, vel, scale, active)
	, m_DebounceTimer(0)
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

void SeaCreature::setDebounceTimer(float timer)
{
    this->m_DebounceTimer = timer;
}

void SeaCreature::setHealth(int m_health)
{
    this->m_health = m_health;
}


Vector3 SeaCreature::cohesion(Vector3 playerpos, Vector3 playertarget)
{
	if (!playerpos.IsZero() || !playertarget.IsZero())
	{

		Vector3 tv = (playertarget * -1).Normalized() * 30;
		Vector3 behindPos = playerpos + tv;
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