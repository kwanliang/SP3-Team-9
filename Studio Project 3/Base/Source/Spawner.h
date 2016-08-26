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
static int g_MaxMinnow = 40;
static int g_MaxPufferfish = 30;
static int g_MaxFCrab = 30;
static int g_MaxChimera = 30;
static int g_MaxCuttlefish = 30;

class Spawner
{
private:
    bool m_IsSpawn;

public:
    Spawner();
    ~Spawner();

    bool getIsSpawn();
    void setIsSpawn(bool m_IsSpawn);

    void CheckCount(int CurrentCount, int MaxCount);
};

#endif