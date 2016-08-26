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
        DEAD,

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
    
    // Grab Attack
    float m_rotateGrabTentacle;
    float m_rotateGrabInnerTentacle;
    bool m_changeGrab;
    bool m_changeInnerGrab;
    bool m_isGrab;
    bool m_Grabbed;

    GiantSquid();
    GiantSquid(float m_bounceTime, bool m_isBounceUp, GIANTSQUID_BEHAVIORSTATE state, int m_health, Vector3 m_LastHitPos, int m_LastDamage, BOSS_TYPE bossType, OBJECT_TYPE objectType, Vector3 pos, Vector3 vel, Vector3 scale, bool active);
    ~GiantSquid();

    void AnimateIdle();
    void AnimateSpinAttack();
    void AnimateInkAttack();
    void AnimateGrabAttack();
    void AnimateDead();
    float LookAtPlayer(Vector3 playerPos);
    void ChasePlayer(Vector3 playerPos);
    void ShootInk(Vector3 playerPos);

    GiantSquidTentacle tentacle1_1;
    GiantSquidTentacle tentacle1_2;
    GiantSquidTentacle tentacle1_3;
    GiantSquidTentacle tentacle1_4;

    GiantSquidTentacle tentacle2_1;
    GiantSquidTentacle tentacle2_2;
    GiantSquidTentacle tentacle2_3;
    GiantSquidTentacle tentacle2_4;

    GiantSquidTentacle tentacle3_1;
    GiantSquidTentacle tentacle3_2;
    GiantSquidTentacle tentacle3_3;
    GiantSquidTentacle tentacle3_4;

    GiantSquidTentacle tentacle4_1;
    GiantSquidTentacle tentacle4_2;
    GiantSquidTentacle tentacle4_3;
    GiantSquidTentacle tentacle4_4;

    GiantSquidTentacle tentacle5_1;
    GiantSquidTentacle tentacle5_2;
    GiantSquidTentacle tentacle5_3;
    GiantSquidTentacle tentacle5_4;

    GiantSquidTentacle tentacle6_1;
    GiantSquidTentacle tentacle6_2;
    GiantSquidTentacle tentacle6_3;
    GiantSquidTentacle tentacle6_4;

    float getSquidLookAt();
    void setSquidLookAt(float m_SquidLookAt);

    bool getIsDead();
    void setIsDead(bool IsDead);

    Vector3 getSquidView();
    void setSquidView(Vector3 m_SquidView);
};

#endif