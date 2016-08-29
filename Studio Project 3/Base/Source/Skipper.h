#ifndef SKIPPER_H
#define SKIPPER_H

#include "GameObject.h"

class Skipper : public GameObject
{
private:
    int m_health;
    int m_damage;
    int m_baseDamage;
	GameObject* target;
    double m_TimerReceieveDamage;
    bool m_isDead;

public:
	enum BoostStatus
	{
		BOOST_READY = 0,
		BOOST_ACTIVE,
		BOOST_RECHARGING,
	};

    Skipper();
    ~Skipper();

    int getHealth();
    void setHealth(int m_health);

    int getDamage();
    void setDamage(int m_damage);

    int getBaseDamage();
    void setBaseDamage(int m_baseDamage);

	GameObject* getTarget();
	void setTarget(GameObject *go);

    double getTimerReceieveDamage();
    void setTimerReceieveDamage(double m_TimerReceieveDamage);

    int randomDamage(int m_damage, int m_baseDamage);

    bool getIsDead();
    void setIsDead(bool m_isDead);

	float stamina;
	BoostStatus boostStatus;
};

#endif