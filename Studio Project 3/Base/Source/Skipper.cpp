#include "Skipper.h"

Skipper::Skipper()
	: m_health(0)
	, m_damage(0)
	, m_baseDamage(0)
	, stamina(100)
	, boostStatus(BOOST_READY)
    , GameObject(PLAYER, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1), false)
{

}

Skipper::Skipper(int m_health, int m_damage, int m_baseDamage, OBJECT_TYPE objectType, Vector3 pos, Vector3 vel, Vector3 scale, bool active)
    : m_health(m_health)
    , m_damage(m_damage)
    , m_baseDamage(m_baseDamage)
    , GameObject(objectType, pos, vel, scale, active)
{

}

Skipper::~Skipper()
{

}

int Skipper::getHealth()
{
    return m_health;
}

void Skipper::setHealth(int m_health)
{
    this->m_health = m_health;
}

int Skipper::getDamage()
{
    return this->m_damage;
}

void Skipper::setDamage(int m_damage)
{
    this->m_damage = m_damage;
}

int Skipper::getBaseDamage()
{
    return this->m_baseDamage;
}

void Skipper::setBaseDamage(int m_baseDamage)
{
    this->m_baseDamage = m_baseDamage;
}

GameObject Skipper::getTarget()
{
	return this->target;
}

void Skipper::setTarget(GameObject *go)
{
	this->target = *go;
}

int Skipper::randomDamage(int m_damage, int m_baseDamage)
{
    return Math::RandIntMinMax(m_baseDamage, m_damage + m_baseDamage);
}