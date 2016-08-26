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
    ~Minnow();

    Vector3 cohesion(Minnow* minnowLeader);

    float getpanicTime();
    void setpanicTime(float panicTime);

    bool getisLeader();
    void setisLeader(bool m_isLeader);
};

#endif