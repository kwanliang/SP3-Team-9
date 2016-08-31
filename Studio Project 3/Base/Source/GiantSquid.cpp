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
    , Boss()
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
    for (int i = 0; i < 6; ++i)
        delete tentacle[i];
}

/******************************************************************************/
/*!
\brief
Animate GiantSquid when state is IDLE
*/
/******************************************************************************/

void GiantSquid::AnimateIdle()
{
    if (m_rotateTentacle < 45.f && !m_isTentacleUp)
    {
        this->m_rotateTentacle += 0.5f;
    }
    else
    {
        this->m_isTentacleUp = true;
        if (m_rotateTentacle > 0.f && m_isTentacleUp)
        {
            m_rotateTentacle -= 0.5f;
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
        for (int i = 0; i < 6; ++i)
        {
            this->tentacle[i]->setTentaclePos(
                Vector3(this->tentacle[i]->getTentaclePos().x,
                this->tentacle[i]->getTentaclePos().y - 1.f,
                this->tentacle[i]->getTentaclePos().z));
        }
    }
    else
    {
        this->m_isSquidUp = true;
        if (m_translateSquid < 0.f && m_isSquidUp)
        {
            this->pos.y += 1.f;
            m_translateSquid += 1.f;
            this->collision.m_position.y += 1.f;
            for (int i = 0; i < 6; ++i)
            {
                this->tentacle[i]->setTentaclePos(
                    Vector3(this->tentacle[i]->getTentaclePos().x,
                    this->tentacle[i]->getTentaclePos().y + 1.f,
                    this->tentacle[i]->getTentaclePos().z));
            }
        }
        else
        {
            this->m_isSquidUp = false;
            this->m_ChangeState = true;
        }
    }

}

/******************************************************************************/
/*!
\brief
Animate GiantSquid when state is SPINATTACK
*/
/******************************************************************************/

void GiantSquid::AnimateSpinAttack()
{
    if (m_rotateTentacle < 45.f && !m_isTentacleUp)
    {
        m_rotateTentacle += 0.5f;
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

            if (m_rotateTentacle > 0.f && m_isTentacleUp && m_rotateComplete)
                m_rotateTentacle -= 0.5f;
            else
            {
                m_isTentacleUp = false;
                m_rotateComplete = false;
            }

            if (!m_isTentacleUp && !m_rotateComplete)
                this->m_ChangeState = true;
        }
    }
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

    if (!this->pos.IsZero())
        wantView.Normalize();

    this->m_SquidLookAt = Math::RadianToDegree(acos(initView.Dot(wantView)));

    Vector3 Crossed = initView.Cross(wantView);
    if (Crossed.Dot(normal) < 0)
    {
        this->m_SquidLookAt *= -1;
    }

    for (int i = 1; i < 7; ++i)
    {
        float x = 0.15 * Math::RadianToDegree(cos(Math::DegreeToRadian(-m_SquidLookAt + i * 55)));
        float z = 0.15 * Math::RadianToDegree(sin(Math::DegreeToRadian(-m_SquidLookAt + i * 55)));

        this->tentacle[i - 1]->collision.m_position = Vector3(pos.x - x, pos.y, pos.z - z);
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
            if (m_isSpinning)
            {
                this->pos += DirVec * m_moveSpeed;
                this->collision.m_position += DirVec * m_moveSpeed;

                for (int i = 0; i < 6; ++i)
                    this->tentacle[i]->collision.m_position += DirVec * m_moveSpeed;
                if (m_moveSpeed < 2.0f)
                    m_moveSpeed += .1f;
            }
            else if (!m_isSpinning)
            {
                this->pos += DirVec * .5f;
                this->collision.m_position += DirVec * .5f;

                for (int i = 0; i < 6; ++i)
                    this->tentacle[i]->collision.m_position += DirVec * m_moveSpeed;
            }
            else
            {
                m_moveSpeed = 1.f;
            }
        }
    }
}

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