#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"
#include "DamageText.h"

struct GameObject
{
    enum OBJECT_TYPE
    {
        PLAYER,
        PROJECTILE,
        SEACREATURE,
        BOSS,
        CAPTURED,
        CORAL,

        TYPE_TOTAL
    };
    OBJECT_TYPE objectType;

    Vector3 pos;
    Vector3 vel;
    Vector3 scale;

	float StunDuration;

    bool active;
    bool inRange;

    GameObject();
    ~GameObject();
};

#endif