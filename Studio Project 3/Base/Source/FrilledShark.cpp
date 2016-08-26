#include "FrilledShark.h"

FrilledShark::FrilledShark()
{
	m_health = 1000;
	bossType = FRILLEDSHARK;
	pos = Vector3(0, 300, 0);
	vel = (0, 0, 0);
	scale = Vector3(50, 50, 50);
	active = true;
	m_node[0].pos = pos;
	m_state = IDLE;

	m_node[1].pos = m_node[0].pos;
	m_node[1].pos.z = m_node[0].pos.z - 140;

	for (unsigned i = 2; i < 4; i++)
	{
		m_node[i].pos = m_node[i-1].pos;
		m_node[i].pos.z = m_node[i - 1].pos.z -80;

	}

	m_node[4].pos = m_node[3].pos;
	m_node[4].pos.z = m_node[3].pos.z;

}

FrilledShark::FrilledShark(int m_health,BOSS_TYPE bossType,
Vector3 pos, Vector3 vel, Vector3 scale, bool active)
{
	m_health = 1000;
	bossType = FRILLEDSHARK;
	pos = (0, 400, 0);
	vel = (0, 0, 0);
	scale = (50, 50, 50);
	active = true;
}

void FrilledShark::UpdateFrilledShark(double dt)
{
	switch (m_state)
	{
	default:
		break;
	}










	Vector3 direction = Vector3(Math::RadianToDegree(cos(Math::DegreeToRadian(-m_node[0].yaw+90))), 0, Math::RadianToDegree(sin(Math::DegreeToRadian(-m_node[0].yaw+90))));//get vel from direction head is facing
	vel = direction.Normalized() * 10;//update velocity
	AnimateFrilledShark(dt);
	//pos += vel*dt;
	m_node[0].pos += vel*dt*10;


}

void FrilledShark::AnimateFrilledShark(double dt)
{

	float zhead = Math::RadianToDegree(sin(Math::DegreeToRadian(-m_node[0].yaw + 90)))*0.65;
	float xhead = Math::RadianToDegree(cos(Math::DegreeToRadian(-m_node[0].yaw + 90)))*0.65;

	//std::cout << y << std::endl;
	//std::cout << x << std::endl;
	m_node[1].pos.x = m_node[0].pos.x - xhead;
	m_node[1].pos.z = m_node[0].pos.z - zhead;

	for (unsigned i = 1; i < 5; i++)
	{

		//if (m_node[0].yaw != m_node[i].yaw)
		//m_node[i].yaw += dt * 10;



		if (m_node[i].yaw < m_node[i - 1].yaw)
		{

			float rotation = Math::Clamp((float)(m_node[i].yaw + dt * 8 * i), (m_node[i - 1].yaw) - 30, (m_node[i - 1].yaw));

			/*		if (i == 2)
			std::cout << rotation << std::endl;*/

			m_node[i].yaw = rotation;

		}
		if (m_node[i].yaw > m_node[i - 1].yaw)
		{
			float rotation = Math::Clamp((float)(m_node[i].yaw - dt * 8 * i), (m_node[i - 1].yaw), (m_node[i - 1].yaw) + 30);
			m_node[i].yaw = rotation;
		}
		//std::cout << "1..." << m_node[1].yaw << std::endl;
		//std::cout << "2..." << m_node[2].yaw << std::endl;

		if (i == 1)
			continue;




		/*if (m_node[i - 1].yaw -m_node[i].yaw > 10)
		m_node[i].yaw += dt *10;
		if (m_node[i - 1].yaw - m_node[i].yaw < -10)
		m_node[i].yaw -= dt * 10;*/



		float z_com = Math::RadianToDegree(sin(Math::DegreeToRadian(-m_node[i - 1].yaw + 90)))*1.6;
		float x_com = Math::RadianToDegree(cos(Math::DegreeToRadian(-m_node[i - 1].yaw + 90)))*1.6;

		m_node[i].pos.x = m_node[i - 1].pos.x - x_com;
		m_node[i].pos.z = m_node[i - 1].pos.z - z_com;

	}

}