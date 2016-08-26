#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "GameObject.h"

class Projectile : public GameObject
{
public:
    enum PROJECTILE_TYPE
    {
        BULLET,
        INK,
		PBULLET,

        TYPE_TOTAL
    };
    PROJECTILE_TYPE projectileType;

    Projectile();
    Projectile(PROJECTILE_TYPE projectileType, OBJECT_TYPE objectType, Vector3 pos, Vector3 vel, Vector3 scale, bool active);
    ~Projectile();
};

#endif