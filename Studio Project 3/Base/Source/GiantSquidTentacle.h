#ifndef GIANTSQUIDTENTACLE_H
#define GIANTSQUIDTENTACLE_H

#include "Vector3.h"
#include "CollisionManager.h"

class GiantSquidTentacle
{
protected:
    Vector3 m_TentaclePos;
    Vector3 m_scale;
    int m_health;


public:
    GiantSquidTentacle();
    virtual ~GiantSquidTentacle();

    Vector3 getTentaclePos();
    void setTentaclePos(Vector3 m_TentaclePos);

    Vector3 getScale();
    void setScale(Vector3 m_scale);

    int getHealth();
    void setHealth(int m_health);

    bool m_active;
    //bool getActive();
    //void setActive(bool m_active);

    hitbox collision;
};

#endif