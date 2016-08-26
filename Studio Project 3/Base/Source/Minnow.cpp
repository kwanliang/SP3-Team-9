#include "Minnow.h"

Minnow::Minnow()
    : state(FLOCK)
    , m_isLeader(false)
    , panicTime(0.f)
    , SeaCreature()
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