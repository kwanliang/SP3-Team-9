#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "GameObject.h"

class Projectile : public GameObject
{
private:
    double m_lifetime;

public:
    enum PROJECTILE_TYPE
    {
		PBULLET,
        SBULLET,
        INK,

        TYPE_TOTAL
    };
    PROJECTILE_TYPE projectileType;

    Projectile();
    ~Projectile();
	Vector3 rotate;
    double getLifetime();
    void setLifetime(double m_lifetime);
};

#endif