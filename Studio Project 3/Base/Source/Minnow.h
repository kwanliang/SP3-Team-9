#ifndef MINNOW_H
#define MINNOW_H

#include "SeaCreature.h"

static float g_MinnowAttractRange = 15000.f;

class Minnow : public SeaCreature
{
private:
    bool m_isLeader;
    float panicTime;

public:
    enum MINNOW_BEHAVIORSTATE
    {
        FLOCK,
        COLLIDING,
        FLEE,
        STATE_TOTAL,
    };
    MINNOW_BEHAVIORSTATE state;

    Minnow();
    Minnow(MINNOW_BEHAVIORSTATE state, bool m_isLeader, float panicTime, int m_health, SEACREATURE_TYPE seaType, OBJECT_TYPE objectType, Vector3 pos, Vector3 vel, Vector3 scale, bool active);
    ~Minnow();

    Vector3 cohesion(Minnow* minnowLeader);

    float getpanicTime();
    void setpanicTime(float panicTime);

    bool getisLeader();
    void setisLeader(bool m_isLeader);
};

#endif