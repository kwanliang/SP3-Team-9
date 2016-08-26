#include "Boss.h"
#include "Mtx44.h"

Boss::Boss()
	: m_health(0)
	, bossType(GIANTSQUID)
	, GameObject(BOSS, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1), false)
{
}

Boss::Boss(int m_health, BOSS_TYPE bossType, OBJECT_TYPE objectType, Vector3 pos, Vector3 vel, Vector3 scale, bool active)
    : m_health(m_health)
    , bossType(bossType)
    , GameObject(objectType, pos, vel, scale, active)
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

void Boss::RotateDirection(const float &degrees)
{
	Mtx44 rot;
	rot.SetToRotation(degrees, 0, 1, 0);
	direction = rot * direction;
}

float Boss::GetOrientation()
{
	return Math::RadianToDegree(atan2(-direction.z, direction.x));
}