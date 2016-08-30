#include "Isopod.h"
#include "SharedData.h"

Isopod::Isopod()
{
	m_health = 1000;
	bossType = ISOPOD;
	pos = Vector3(0, 0, 0);
	vel = (0, 0, 0);
	scale = Vector3(80, 80, 80);
	active = true;
	m_state = AGGRO;
	m_SpawnIsopodDrone = false;
	m_SpawnBufferTime = 0.0f;
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

	m_targetnest = & m_nest_A;

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
	float h = 350.f * ReadHeightMap(hmap, pos.x / 3000.f, pos.z / 3000.f) + 16;//update height
	if (pos.y < h)
		pos.y += dt * 10;
	else if (pos.y > h)
		pos.y -= dt * 10;

	Vector3 displacement = m_targetnest->pos - pos;

	vel = displacement.Normalized();
	if (displacement.LengthSquared() < 100 * 100)
	{
		if (m_targetnest == &m_nest_A)
			m_targetnest = &m_nest_B;
		else
			m_targetnest = &m_nest_A;
	}


    switch (m_state)
    {
    case IDLE:
    {  
		m_SpawnIsopodDrone = false;
        

     break;
    }
    case AGGRO:
    {
		speed = 30;
        /*	vel = displacement.Normalized();
        if (displacement.LengthSquared() < 100 * 100)
        {
        if (m_targetnest == &m_nest_A)
        m_targetnest = &m_nest_B;
        else
        m_targetnest = &m_nest_A;
        }*/

        m_SpawnBufferTime += dt;
        if (m_SpawnBufferTime > 1.0)
        {
            m_SpawnIsopodDrone = true;
            m_SpawnBufferTime = 1.f;
        }
        break;
    }
    case CHARGE:
    {
        break;
    }
    case COLLIDING:
    {
        break;
    }
    }

	AnimateIsopod(dt);
	pos += vel*dt*speed;


}

void Isopod::AnimateIsopod(double dt)
{
	for (unsigned i = 0; i < 6; i++)
	{
		m_Rleg[i].roll += dt*4;
		m_Lleg[i].roll += dt*4;

	}
	
}

bool Isopod::getSpawnIsopodDrone()
{
    return this->m_SpawnIsopodDrone;
}

void Isopod::setSpawnIsopodDrone(bool m_SpawnIsopodDrone)
{
    this->m_SpawnIsopodDrone = m_SpawnIsopodDrone;
}

double Isopod::getSpawnBufferTime()
{
    return this->m_SpawnBufferTime;
}

void Isopod::setSpawnBufferTime(double m_SpawnBufferTime)
{
    this->m_SpawnBufferTime = m_SpawnBufferTime;
}