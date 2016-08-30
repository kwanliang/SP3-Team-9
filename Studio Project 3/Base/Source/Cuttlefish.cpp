#include "Cuttlefish.h"

Cuttlefish::Cuttlefish()
{
	SeaCreature();
	rotate = 0;
	ctstate = IDLE;
	jet = false;
    m_IsShoot = false;
    m_ShootBufferTime = 0.0;
}

Cuttlefish::~Cuttlefish()
{

}

void Cuttlefish::UpdateCuttle(double dt)
{
	switch (ctstate)
	{
	case IDLE :
	{
		if (!jet)
		{
			if (vel.Length() > 1)
			{
				vel -= vel.Normalized()*dt * 35;

				//std::cout << decel << std::endl;
			}
			else
				jet = true;
		}
		else if (jet)
		{
			vel = Vector3(Math::RandFloatMinMax(-50, 50), Math::RandFloatMinMax(-20, 20), Math::RandFloatMinMax(-50, 50));
			jet = false;
		}

		rotate = Math::RadianToDegree(atan2(vel.x, vel.z));

        if ((SharedData::GetInstance()->SD_PlayerPos - pos).LengthSquared() < 30000)
            ctstate = CUTTLE_STATE::ATTACKING;

        break;
	}
	case SINK :
	{	
		if (pos.y < 400)
			ctstate = IDLE;
        break;
	}
    case ATTACKING:
    {
        m_ShootBufferTime += dt;
        if (m_ShootBufferTime > 2.0)
        {
            m_IsShoot = true;
            m_ShootBufferTime = 0.0;
        }

        if ((SharedData::GetInstance()->SD_PlayerPos - pos).LengthSquared() >= 30000)
            ctstate = CUTTLE_STATE::IDLE;

        break;
    }
	}
	
	
	if (pos.y > 700)
	{
		vel = Vector3(Math::RandFloatMinMax(-60, 50), Math::RandFloatMinMax(-20, -8), Math::RandFloatMinMax(-30, 60));
		ctstate = SINK;
	}
	
	
	hitbox2::updatehitbox(collision,pos);
	pos += vel*dt*2;
}

bool Cuttlefish::getIsShoot()
{
    return this->m_IsShoot;
}

void Cuttlefish::setIsShoot(bool m_IsShoot)
{
    this->m_IsShoot = m_IsShoot;
}

bool Cuttlefish::getShootBufferTime()
{
    return this->m_ShootBufferTime;
}

void Cuttlefish::setShootBufferTime(bool m_ShootBufferTime)
{
    this->m_ShootBufferTime = m_ShootBufferTime;
}