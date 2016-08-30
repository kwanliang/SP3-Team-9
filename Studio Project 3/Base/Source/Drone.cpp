#include "Drone.h"
#include "SharedData.h"

Drone::Drone()
{
	SeaCreature();
	m_hitbox = hitbox::generatehitbox(Vector3(0, 0, 0), 10, 10, 10, NULL);
	vel = Vector3(Math::RandFloatMinMax(-50, 50), Math::RandFloatMinMax(-20, 20), Math::RandFloatMinMax(-50, 50));
	m_state = STRAFE;
}

Drone::~Drone()
{

}

void Drone::UpdateDrone(double dt, std::vector<unsigned char> hmap)
{
	Vector3 Ppos = SharedData::GetInstance()->SD_PlayerPos;
	Vector3 P_displacement = Ppos - pos;
	Vector3 I_displacement = m_location - pos;
	switch (m_state)
	{
	case ATTACK:
		vel = P_displacement.Normalized();
		//vel.x += Math::RandFloatMinMax(0, 3);
	/*	if (P_displacement.LengthSquared() > 10 * 10)
		{*/
		pos += vel*dt*50;
		//}
		break;
	case STRAFE:
	{
		//vel = (Math::RandFloatMinMax(-50, 50), Math::RandFloatMinMax(-20, 20), Math::RandFloatMinMax(-50, 50));
		if (P_displacement.LengthSquared() < 300 * 300)
		{
			m_state = ATTACK;
		}

		pos += vel*dt *15;
		break;
	}
	case RETREAT:
	{
		vel = I_displacement.Normalized() * 20;
		pos += vel*dt * 10;
		if (I_displacement.LengthSquared() < 50 * 50)
		{
			vel = Vector3(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-5, 5), Math::RandFloatMinMax(-10, 10));
			m_state = STRAFE;

		}

	}

	}

	if (I_displacement.LengthSquared() > 400 * 400)
	{
		
		m_state = RETREAT;

	}
	if (pos.y < 350.f * ReadHeightMap(hmap, pos.x / 3000.f, pos.z / 3000.f))
	{
		vel.y = -vel.y;
		pos.y += 3;
		
	}
//
//	//vel = P_displacement.Normalized();
	hitbox::updatehitbox(m_hitbox, pos);
}

