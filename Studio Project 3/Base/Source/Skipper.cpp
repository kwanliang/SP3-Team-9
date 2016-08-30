#include "Skipper.h"

Skipper::Skipper()
	: m_health(0)
	, m_damage(0)
	, m_baseDamage(0)
	, stamina(100)
	, boostStatus(BOOST_READY)
    , m_isDead(false)
    , GameObject()
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
    if (this->m_health > g_MaxSkipperHealth)
        this->m_health = g_MaxSkipperHealth;
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

GameObject* Skipper::getTarget()
{
	return this->target;
}

void Skipper::setTarget(GameObject *go)
{
	this->target = go;
}

double Skipper::getTimerReceieveDamage()
{
    return this->m_TimerReceieveDamage;
}

void Skipper::setTimerReceieveDamage(double m_TimerReceieveDamage)
{
    this->m_TimerReceieveDamage = m_TimerReceieveDamage;
}

int Skipper::randomDamage(int m_damage, int m_baseDamage)
{
    return Math::RandIntMinMax(m_baseDamage, m_damage + m_baseDamage);
}

bool Skipper::getIsDead()
{
    return this->m_isDead;
}

void Skipper::setIsDead(bool m_isDead)
{
    this->m_isDead = m_isDead;
}

int Skipper::getHealthPackCount()
{
    return this->m_HealthPackCount;
}

void Skipper::setHealthPackCount(int m_HealthPackCount)
{
    this->m_HealthPackCount = m_HealthPackCount;
    if (this->m_HealthPackCount > 3)
        this->m_HealthPackCount = 3;
}

int Skipper::getStaminaPackCount()
{
    return this->m_StaminaPackCount;
}

void Skipper::setStaminaPackCount(int m_StaminaPackCount)
{
    this->m_StaminaPackCount = m_StaminaPackCount;
    if (this->m_StaminaPackCount > 3)
        this->m_StaminaPackCount = 3;
}