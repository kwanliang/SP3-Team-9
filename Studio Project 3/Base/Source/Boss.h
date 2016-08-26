#ifndef BOSS_H
#define BOSS_H

#include "GameObject.h"
#include "CollisionManager.h"

class Boss : public GameObject
{
protected:
    int m_health;

public:
    enum BOSS_TYPE
    {
        GIANTSQUID = 0,
        GIANTSQUIDTENTACLE,
        FRILLEDSHARK,
		GIANTCRAB,
        TYPE_TOTAL,
    };

    BOSS_TYPE bossType;

    Boss();
    ~Boss();

    int getHealth();
    void setHealth(int m_health);
    bool isdead;
    hitbox collision;
};

#endif