/******************************************************************************/
/*!
\file	GiantSquid.h
\author Lee Kwan Liang
\par	email: 155117G@mymail.nyp.edu.sg
\brief
Class to allow spawning of objects
*/
/******************************************************************************/

#ifndef SPAWNER_H
#define SPAWNER_H

#include "Vector3.h"

/******************************************************************************/
/*!
Class Spawner:
\brief
Defines spawner and its methods
*/
/******************************************************************************/

static int g_MaxMinnowLeader = 3;
static int g_MaxMinnow = 30;

class Spawner
{
private:
    Vector3 m_pos;
    Vector3 m_MinRange;
    Vector3 m_MaxRange;
    bool m_IsSpawn;

public:
    Spawner();
    ~Spawner();

    Vector3 getPos();
    void setPos(Vector3 m_pos);

    bool getIsSpawn();
    void setIsSpawn(bool m_IsSpawn);

    Vector3 getMinRange();
    void setMinRange(Vector3 m_MinRange);

    Vector3 getMaxRange();
    void setMaxRange(Vector3 m_MaxRange);

    void CheckCount(int CurrentCount, int MaxCount);
};

#endif