#include "Minnow.h"

Minnow::Minnow()
    : state(FLOCK)
    , m_isLeader(false)
    , panicTime(0.f)
    , SeaCreature(0, MINNOW, SEACREATURE, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1), false)
{

}

Minnow::Minnow(MINNOW_BEHAVIORSTATE state, bool m_isLeader, float panicTime, int m_health, SEACREATURE_TYPE seaType, OBJECT_TYPE objectType, Vector3 pos, Vector3 vel, Vector3 scale, bool active)
    : state(state)
    , m_isLeader(m_isLeader)
    , panicTime(panicTime)
    , SeaCreature(m_health, seaType, objectType, pos, vel, scale, active)
{

}

Minnow::~Minnow()
{

}

Vector3 Minnow::cohesion(Minnow* minnowLeader)
{
    if (!minnowLeader->pos.IsZero() || !minnowLeader->vel.IsZero())
    {
        Vector3 tv = (minnowLeader->vel * -1).Normalized() * 30;
        Vector3 behindPos = minnowLeader->pos + tv;
        behindPos = (behindPos - this->pos).Normalized();

        return behindPos;
    }
    return Vector3(0, 0, 0);
}



float Minnow::getpanicTime()
{
    return this->panicTime;
}

void Minnow::setpanicTime(float panicTime)
{
    this->panicTime = panicTime;
}

bool Minnow::getisLeader()
{
    return this->m_isLeader;
}

void Minnow::setisLeader(bool m_isLeader)
{
    this->m_isLeader = m_isLeader;
}