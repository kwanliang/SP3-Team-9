#ifndef SEACREATURE_H
#define SEACREATURE_H

#include "GameObject.h"
#include "Vector3.h"
#include "CollisionManager.h"
#include "Spawner.h"

static float g_distFromSeperation = 200.f;
static float g_distFromSeperationIntensity = 400.f;
static float g_distFromAlignment = 400.f;

class SeaCreature : public GameObject
{
protected:
    int m_health;
    float m_DebounceTimer;
    bool m_canVacuum;
	bool isVacuum;

public:
    enum SEACREATURE_TYPE
    {
        MINNOW = 0,
        PUFFER,
        FCRAB,
        CHIMERA,
        CUTTLE,
		DRONE,
        TYPE_TOTAL,
    };
    SEACREATURE_TYPE seaType;

    SeaCreature();
    ~SeaCreature();

    Vector3 cohesion(Vector3 playerpos, Vector3 playertarget);
    Vector3 seperation(Vector3 repelVector);
    Vector3 alignment(Vector3 forceVector);

    int getHealth();
    float getDebounceTimer();
    bool getCanVacuum();
	bool isstunned = false;
	bool getisVacuum();

    void setHealth(int m_health);
    void setDebounceTimer(float timer);
    void setCanVacuum(bool m_canVacuum);
	void setisVacuum(bool isVacuum);

    hitbox	aabb;
    hitbox2 collision;
};

#endif