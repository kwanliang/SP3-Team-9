#include "Isopod.h"
#include "SharedData.h"

Isopod::Isopod()
{
	m_health = 1000;
	bossType = ISOPOD;
	pos = Vector3(0, 500, 0);
	vel = (0, 0, 0);
	scale = Vector3(80, 80, 80);
	active = true;
	m_state = IDLE;

	//for (unsigned i = 0; i < 6; i++)
	//{
	//	m_Rleg[i].roll = i/5;
	//	m_Lleg[i].roll =  i/5;
	//}
	m_Rleg[0].roll = 0.2;
	m_Rleg[1].roll = 0.5;
	m_Rleg[2].roll = 0.8;
	m_Rleg[3].roll = 1.1;
	m_Rleg[4].roll = 1.4;
	m_Rleg[5].roll = 1.7;


	m_Lleg[0].roll = 1.7;
	m_Lleg[1].roll = 1.4;
	m_Lleg[2].roll = 1.1;
	m_Lleg[3].roll = 0.8;
	m_Lleg[4].roll = 0.5;
	m_Lleg[5].roll = 0.2;

	//m_Lwhisker = hitbox2::generatehitbox(Vector3(0, 0, 0), 20, 20, 20);
	//m_Rwhisker = hitbox2::generatehitbox(Vector3(0, 0, 0), 20, 20, 20);
}

Isopod::Isopod(int m_health, BOSS_TYPE bossType,
	Vector3 pos, Vector3 vel, Vector3 scale, bool active)
{

}

void Isopod::UpdateIsopod(double dt, std::vector<unsigned char> hmap)
{
	float speed = 25;
	Vector3 P_pos = SharedData::GetInstance()->SD_PlayerPos;
	Vector3 P_displacement = P_pos - pos;

	
	AnimateIsopod(dt);
	//pos += vel*dt;



}

void Isopod::AnimateIsopod(double dt)
{
	for (unsigned i = 0; i < 6; i++)
	{
		m_Rleg[i].roll += dt*4;
		m_Lleg[i].roll += dt*4;

	}
	
	

}
