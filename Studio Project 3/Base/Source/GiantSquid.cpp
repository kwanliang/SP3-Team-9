/******************************************************************************/
/*!
\file	GiantSquid.h
\author Lee Kwan Liang
\par	email: 155117G@mymail.nyp.edu.sg
\brief
Class to define boss, Giant Squid
*/
/******************************************************************************/

#include "GiantSquid.h"

/******************************************************************************/
/*!
\brief
GiantSquid default constructor
*/
/******************************************************************************/

GiantSquid::GiantSquid()
    : state(IDLE)
    , Boss(0, GIANTSQUID, BOSS, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1), false)
{

}

/******************************************************************************/
/*!
\brief
GiantSquid overloaded constructor
*/
/******************************************************************************/

GiantSquid::GiantSquid(float m_bounceTime, bool m_isBounceUp, GIANTSQUID_BEHAVIORSTATE state, int m_health, Vector3 m_LastHitPos, int m_LastDamage, BOSS_TYPE bossType, OBJECT_TYPE objectType, Vector3 pos, Vector3 vel, Vector3 scale, bool active)
    : state(state)
    , Boss(m_health, bossType, objectType, pos, vel, scale, active)
{

}

/******************************************************************************/
/*!
\brief
GiantSquid default deconstructor
*/
/******************************************************************************/

GiantSquid::~GiantSquid()
{

}

/******************************************************************************/
/*!
\brief
Animate GiantSquid when state is IDLE
*/
/******************************************************************************/

void GiantSquid::AnimateIdle()
{
    if (m_rotateSpinTentacle < 45.f && !m_isTentacleUp)
    {
        this->m_rotateSpinTentacle += 0.5f;
    }
    else
    {
        this->m_isTentacleUp = true;
        if (m_rotateSpinTentacle > 0.f && m_isTentacleUp)
        {
            m_rotateSpinTentacle -= 0.5f;
        }
        else
        {
            this->m_isTentacleUp = false;
        }
    }

    if (m_translateSquid > -90.f && !m_isSquidUp)
    {
        this->pos.y -= 1.f;
        this->m_translateSquid -= 1.f;
        this->collision.m_position.y -= 1.f;
        this->tentacle1_1.collision.m_position.y -= 1.f;
        this->tentacle1_2.collision.m_position.y -= 1.f;
        this->tentacle1_3.collision.m_position.y -= 1.f;
        this->tentacle1_4.collision.m_position.y -= 1.f;
    }
    else
    {
        this->m_isSquidUp = true;
        if (m_translateSquid < 0.f && m_isSquidUp)
        {
            this->pos.y += 1.f;
            m_translateSquid += 1.f;
            this->collision.m_position.y += 1.f;
            this->tentacle1_1.collision.m_position.y += 1.f;
            this->tentacle1_2.collision.m_position.y += 1.f;
            this->tentacle1_3.collision.m_position.y += 1.f;
            this->tentacle1_4.collision.m_position.y += 1.f;
        }
        else
        {
            this->m_isSquidUp = false;
            this->m_ChangeState = true;
        }
    }


    tentacle1_1.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle1_2.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle1_3.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle1_4.setTentacleAnimateRotate(m_rotateSpinTentacle);

    tentacle2_1.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle2_2.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle2_3.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle2_4.setTentacleAnimateRotate(m_rotateSpinTentacle);

    tentacle3_1.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle3_2.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle3_3.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle3_4.setTentacleAnimateRotate(m_rotateSpinTentacle);

    tentacle4_1.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle4_2.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle4_3.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle4_4.setTentacleAnimateRotate(m_rotateSpinTentacle);

    tentacle5_1.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle5_2.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle5_3.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle5_4.setTentacleAnimateRotate(m_rotateSpinTentacle);

    tentacle6_1.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle6_2.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle6_3.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle6_4.setTentacleAnimateRotate(m_rotateSpinTentacle);
}

/******************************************************************************/
/*!
\brief
Animate GiantSquid when state is SPINATTACK
*/
/******************************************************************************/

void GiantSquid::AnimateSpinAttack()
{
    if (m_rotateSpinTentacle < 45.f && !m_isTentacleUp)
    {
        m_rotateSpinTentacle += 0.5f;
    }
    else
    {
        m_isTentacleUp = true;
        if (m_rotateSquid < g_squidSpinValue && m_isTentacleUp && !m_rotateComplete)
        {
            m_rotateSquid += m_spinSpeed;
            m_isSpinning = true;
            m_spinSpeed += 0.2f;
        }
        else
        {
            m_isSpinning = false;
            m_rotateComplete = true;
            m_rotateSquid = 0.f;
            m_spinSpeed = 0.f;

            if (m_rotateSpinTentacle > 0.f && m_isTentacleUp && m_rotateComplete)
                m_rotateSpinTentacle -= 0.5f;
            else
            {
                m_isTentacleUp = false;
                m_rotateComplete = false;
            }

            if (!m_isTentacleUp && !m_rotateComplete)
                this->m_ChangeState = true;
        }
    }

    tentacle1_1.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle1_2.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle1_3.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle1_4.setTentacleAnimateRotate(m_rotateSpinTentacle);

    tentacle2_1.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle2_2.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle2_3.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle2_4.setTentacleAnimateRotate(m_rotateSpinTentacle);

    tentacle3_1.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle3_2.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle3_3.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle3_4.setTentacleAnimateRotate(m_rotateSpinTentacle);

    tentacle4_1.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle4_2.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle4_3.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle4_4.setTentacleAnimateRotate(m_rotateSpinTentacle);

    tentacle5_1.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle5_2.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle5_3.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle5_4.setTentacleAnimateRotate(m_rotateSpinTentacle);

    tentacle6_1.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle6_2.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle6_3.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle6_4.setTentacleAnimateRotate(m_rotateSpinTentacle);
}

/******************************************************************************/
/*!
\brief
Animate GiantSquid when state is INKATTACK
*/
/******************************************************************************/

void GiantSquid::AnimateInkAttack()
{
    if (m_rotateInkTentacle < 30.f && !m_isInkTentacleRotated)
    {
        m_rotateInkTentacle += 0.5f;
    }
    else
    {
        if (m_InkFiredCount < 5 && !m_isShootInk)
        {
            m_InkFiredCount++;
            m_isShootInk = true;
        }
        else
        {
            m_isShootInk = false;
            m_isInkTentacleRotated = true;
            if (m_rotateInkTentacle > 0.f && m_isInkTentacleRotated && !m_isShootInk)
            {
                m_rotateInkTentacle -= 0.5f;
            }
            else
            {
                m_rotateInkTentacle = 0.f;
                m_isInkTentacleRotated = false;
                m_isShootInk = false;
                m_InkFiredCount = 0;
                this->m_ChangeState = true;
            }
        }
    }

    tentacle1_1.setTentacleAnimateRotate(m_rotateInkTentacle);
    tentacle2_1.setTentacleAnimateRotate(m_rotateInkTentacle);
    tentacle3_1.setTentacleAnimateRotate(m_rotateInkTentacle);
    tentacle4_1.setTentacleAnimateRotate(m_rotateInkTentacle);
    tentacle5_1.setTentacleAnimateRotate(m_rotateInkTentacle);
    tentacle6_1.setTentacleAnimateRotate(m_rotateInkTentacle);
}

/******************************************************************************/
/*!
\brief
Animate GiantSquid when state is GRABATTACK
*/
/******************************************************************************/

void GiantSquid::AnimateGrabAttack()
{
    if (m_rotateGrabTentacle < 45.f && !m_isTentacleUp)
    {
        this->m_rotateGrabTentacle += 0.5f;
    }
    else
    {
        if (m_rotateSpinTentacle < 45.f && !m_changeGrab)
        {
            this->m_rotateSpinTentacle += 1.5f;
        }
        else
        {
            this->m_changeGrab = true;
            if (m_rotateSpinTentacle > 0.f && m_changeGrab)
            {
                m_rotateSpinTentacle -= 1.5f;
            }
            else
            {
                m_isTentacleUp = true;
                if (m_rotateGrabTentacle > 0.f && m_isTentacleUp)
                {
                    this->m_rotateGrabTentacle -= 0.5f;
                }
                else
                {
                    m_rotateGrabTentacle = 0.f;
                    m_rotateSpinTentacle = 0.f;
                    m_isTentacleUp = false;
                    m_changeGrab = false;
                    m_ChangeState = true;
                }    
            }
        }
    }

    //if (m_isGrab)
    //{
    //    if (m_rotateGrabInnerTentacle < 45.f)
    //    {
    //        m_rotateGrabInnerTentacle += 1.f;
    //    }
    //    else
    //    {
    //        m_isGrab = false;
    //    }
    //}
    //else if (!m_isGrab && m_rotateGrabInnerTentacle > 0.f)
    //{
    //    m_rotateGrabInnerTentacle -= 1.f;
    //    m_changeGrab = false;
    //    m_Grabbed = true;
    //}

    tentacle1_1.setTentacleAnimateRotate(m_rotateGrabTentacle);
    tentacle1_2.setTentacleAnimateRotate(m_rotateGrabTentacle);
    tentacle1_3.setTentacleAnimateRotate(m_rotateGrabTentacle);
    tentacle1_4.setTentacleAnimateRotate(m_rotateGrabTentacle);
    tentacle2_1.setTentacleAnimateRotate(m_rotateGrabTentacle);
    tentacle2_2.setTentacleAnimateRotate(m_rotateGrabTentacle);
    tentacle2_3.setTentacleAnimateRotate(m_rotateGrabTentacle);
    tentacle2_4.setTentacleAnimateRotate(m_rotateGrabTentacle);

    tentacle1_1.setTentacleAnimateRotate2(m_rotateGrabInnerTentacle);
    tentacle1_2.setTentacleAnimateRotate2(m_rotateGrabInnerTentacle);
    tentacle1_3.setTentacleAnimateRotate2(m_rotateGrabInnerTentacle);
    tentacle1_4.setTentacleAnimateRotate2(m_rotateGrabInnerTentacle);
    tentacle2_1.setTentacleAnimateRotate2(-m_rotateGrabInnerTentacle);
    tentacle2_2.setTentacleAnimateRotate2(-m_rotateGrabInnerTentacle);
    tentacle2_3.setTentacleAnimateRotate2(-m_rotateGrabInnerTentacle);
    tentacle2_4.setTentacleAnimateRotate2(-m_rotateGrabInnerTentacle);

    tentacle3_1.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle3_2.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle3_3.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle3_4.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle4_1.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle4_2.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle4_3.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle4_4.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle5_1.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle5_2.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle5_3.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle5_4.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle6_1.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle6_2.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle6_3.setTentacleAnimateRotate(m_rotateSpinTentacle);
    tentacle6_4.setTentacleAnimateRotate(m_rotateSpinTentacle);

}

/******************************************************************************/
/*!
\brief
Animate GiantSquid when state is DEAD
*/
/******************************************************************************/

void GiantSquid::AnimateDead()
{
    this->m_isDead = true;
}

/******************************************************************************/
/*!
\brief
Function for GiantSquid to face the player

\param playerPos
Vector3 of player's position
*/
/******************************************************************************/

float GiantSquid::LookAtPlayer(Vector3 playerPos)
{
    Vector3 initView(-1, 0, 0);
    Vector3 wantView(playerPos - this->pos);
    Vector3 normal(0, 1, 0);

    if (this->pos != (0, 0, 0))
        wantView.Normalize();

    this->m_SquidLookAt = Math::RadianToDegree(acos(initView.Dot(wantView)));
    Vector3 Crossed = initView.Cross(wantView);
    if (Crossed.Dot(normal) < 0)
    {
        this->m_SquidLookAt *= -1;
    }

    return m_SquidLookAt;
}

/******************************************************************************/
/*!
\brief
Function for GiantSquid to chase the player

\param playerPos
Vector3 of player's position
*/
/******************************************************************************/

void GiantSquid::ChasePlayer(Vector3 playerPos)
{
    Vector3 DirVec = playerPos - this->pos;
    if (playerPos != this->pos && !DirVec.IsZero())
    {
        DirVec.Normalize();
        if ((playerPos - this->pos).LengthSquared() > g_distFromPlayer * g_distFromPlayer)
        {
            if (m_isSpinning || m_changeGrab)
            {
                this->pos += DirVec * m_moveSpeed;
                this->collision.m_position += DirVec * m_moveSpeed;
                if (m_moveSpeed < 1.5f)
                    m_moveSpeed += .05f;
            }
            else if (!m_isSpinning || !m_changeGrab)
            {
                this->pos += DirVec * .5f;
                this->collision.m_position += DirVec * .5f;
            }
            else
            {
                m_moveSpeed = 1.f;
            }
        }
    }
}

/******************************************************************************/
/*!
\brief
Function for GiantSquid to shoot ink at player

\param playerPos
Vector3 of player's position
*/
/******************************************************************************/

//void GiantSquid::ShootInk(Vector3 playerPos)
//{
//    bool InkShotState = false;
//    if (!InkShotState)
//    {
//        InkShotState = true;
//    }
//    else if (InkShotState)
//    {
//        Projectile *po = FetchPO();
//        po->objectType = GameObject::PROJECTILE;
//        po->projectileType = Projectile::BULLET;
//        po->active = true;
//        po->scale.Set(1, 1, 1);
//        po->pos.Set(playerpos.x, playerpos.y, playerpos.z);
//        Vector3 view = (walkCam.GetDir()).Normalized();
//        po->vel.Set(view.x, view.y, view.z);
//        bSPACEstate = false;
//    }
//}

float GiantSquid::getSquidLookAt()
{
    return this->m_SquidLookAt;
}

void GiantSquid::setSquidLookAt(float m_SquidLookAt)
{
    this->m_SquidLookAt = m_SquidLookAt;
}

bool GiantSquid::getIsDead()
{
    return this->m_isDead;
}

void GiantSquid::setIsDead(bool IsDead)
{
    this->m_isDead = IsDead;
}

Vector3 GiantSquid::getSquidView()
{
    return this->m_SquidView;
}

void GiantSquid::setSquidView(Vector3 m_SquidView)
{
    this->m_SquidView = m_SquidView;
}