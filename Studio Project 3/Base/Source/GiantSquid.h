/******************************************************************************/
/*!
\file	GiantSquid.h
\author Lee Kwan Liang
\par	email: 155117G@mymail.nyp.edu.sg
\brief
Class to define boss, Giant Squid
*/
/******************************************************************************/

#ifndef GIANTSQUID_H
#define GIANTSQUID_H

#include "Boss.h"
#include "Mtx44.h"
#include "GiantSquidTentacle.h"

static float g_distFromPlayer = 100.f;
static float g_squidSpinValue = 1440.f;
static float g_inkSpread = 3.f;

/******************************************************************************/
/*!
Class GiantSquid:
\brief
Defines the Boss and its methods
*/
/******************************************************************************/

class GiantSquid : public Boss
{
private:
    float m_SquidLookAt;
    Vector3 m_SquidView;
    bool m_isDead;

public:
    enum GIANTSQUID_BEHAVIORSTATE
    {
        TRANSITION,
        IDLE,
        SPINATTACK,
        INKATTACK,
        GRABATTACK,

        STATE_TOTAL,
    };
    GIANTSQUID_BEHAVIORSTATE state;

    // Animation //
    bool m_ChangeState;

    // Chase Player
    float m_moveSpeed;

    // Idle
    float m_translateSquid;
    bool m_isSquidUp;
    float m_rotateTentacle;

    // Spin Attack
    bool m_isTentacleUp;
    float m_rotateSpinTentacle;
    float m_rotateSquid;
    bool m_rotateComplete;
    bool m_isSpinning;
    float m_spinSpeed;

    // Ink Attack
    bool m_isInkTentacleRotated;
    float m_rotateInkTentacle;
    bool m_isShootInk;
    int m_InkFiredCount;

    GiantSquid();
    ~GiantSquid();

    void AnimateIdle();
    void AnimateSpinAttack();
    void AnimateInkAttack();
    void AnimateDead();
    float LookAtPlayer(Vector3 playerPos);
    void ChasePlayer(Vector3 playerPos);
    void ShootInk(Vector3 playerPos);

    float getSquidLookAt();
    void setSquidLookAt(float m_SquidLookAt);

    bool getIsDead();
    void setIsDead(bool IsDead);

    Vector3 getSquidView();
    void setSquidView(Vector3 m_SquidView);

    GiantSquidTentacle* tentacle[6];
};

#endif