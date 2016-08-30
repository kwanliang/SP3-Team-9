#include "Drone.h"
#include "SharedData.h"

Drone::Drone()
{
	SeaCreature();
	hitbox::generatehitbox(Vector3(0, 0, 0), 10, 10, 10, NULL);
	vel = Vector3(20,20,20);
	m_state = STRAFE;
}

Drone::~Drone()
{

}

void Drone::UpdateDrone(double dt)
{
	//Vector3 Ppos = SharedData::GetInstance()->SD_PlayerPos;
	//Vector3 P_displacement = Ppos - pos;
	pos += vel*dt;
	//switch (m_state)
	//{
	//	/*case ATTACK:
	//		vel = P_displacement.Normalized();
	//		if (P_displacement.LengthSquared() > 10 * 10)
	//		{
	//		pos += vel*dt * 80;
	//		}
	//		break;*/
	//case STRAFE:
	//{
	//	if (P_displacement.LengthSquared() < 30 * 30)
	//	{
	//		//m_state = ATTACK;
	//	}

	//	pos += vel*dt*10;
	//	break;
	//}
	/*case COLLIDE:
	{

	vel = -vel;
	m_state = STRAFE;

	}break;
	}*/
	}

	//vel = P_displacement.Normalized();
	//hitbox::updatehitbox(m_hitbox, pos);

