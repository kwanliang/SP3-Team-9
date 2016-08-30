#include "FrilledShark.h"
#include "SharedData.h"

FrilledShark::FrilledShark()
{
	m_health = 1000;
	bossType = FRILLEDSHARK;
	objectType = GameObject::BOSS;
	pos = Vector3(0, 300, 0);
	vel.Set(0, 0, 0);
	scale = Vector3(50, 50, 50);
	active = true;
	m_node[0].pos = pos;
	m_state = IDLE;
	m_strafeTime = 0;
	this->setHealth(1000);
	m_node[1].pos = m_node[0].pos;
	m_node[1].pos.z = m_node[0].pos.z - 140;

	for (unsigned i = 2; i < 4; i++)
	{
		m_node[i].pos = m_node[i-1].pos;
		m_node[i].pos.z = m_node[i - 1].pos.z -80;

	}
	for (unsigned i = 0; i < 5; i++)
	{
		m_FSbox[i] = hitbox::generatehitbox(Vector3(0, 0, 0), 110, 50, 110 ,NULL);

	}


	m_node[4].pos = m_node[3].pos;
	m_node[4].pos.z = m_node[3].pos.z;


	m_Lwhisker = hitbox2::generatehitbox(Vector3(0, 0, 0), 20, 20, 20);
	m_Rwhisker = hitbox2::generatehitbox(Vector3(0, 0, 0), 20, 20, 20);
}

FrilledShark::~FrilledShark()
{

}

void FrilledShark::UpdateFrilledShark(double dt, std::vector<unsigned char> hmap)
{
	float speed = 25;
	Vector3 P_pos = SharedData::GetInstance()->SD_PlayerPos;
	Vector3 P_displacement = P_pos - pos;

	UpdateWhiskers();
	UpdateHitboxes();
	switch (m_state)
	{
	case IDLE:

		Ljaw_rotate = -10;

		std::cout << "IDLE" << std::endl;


		if (P_displacement.LengthSquared() < 500 * 500)
			m_state = AGGRO;
		break;


	case AGGRO:
	{
		Ljaw_rotate = 5;
		float rotation = Math::RadianToDegree(atan2(P_displacement.x, P_displacement.z));


		if (m_node[0].yaw < rotation)
		m_node[0].yaw = Math::Max((float)(m_node[0].yaw + dt * 10), rotation-5);
		else if(m_node[0].yaw > rotation)
			m_node[0].yaw = Math::Min((float)(m_node[0].yaw - dt * 10), rotation+5);
		
		//m_node[0].yaw = rotation;
		vel.y = P_displacement.y * 10;
		//pos.y = Math::Clamp(pos.y, -200.f, P_pos.y);

		if ( P_displacement.Length() < 400 )
		{
			vel.y = 0;
			m_state = CHARGE;
		}

		std::cout << "AGGRO" << std::endl;
	}	break;
	case CHARGE:
		Ljaw_rotate = 15;

		vel.y = P_displacement.y * 4;
		speed = 120;
		std::cout << "CHARGE" << std::endl;
		if (P_displacement.LengthSquared() > 500 * 500)
			m_state = AGGRO;
		break;
	case STRAFE:
		Ljaw_rotate = 0;
		speed = 30;
		m_strafeTime += dt;
		if (m_strafeTime > 2 && P_displacement.LengthSquared() > 400 * 400)
		{
			m_strafeTime = 0;
			m_state = AGGRO;
		}
		std::cout << "STRAFE" << std::endl;
		break;

	case COLLIDING:
		std::cout << "COLLDIING" << std::endl;
		break;
	}

	//std::cout << P_displacment.LengthSquared() << std::endl;

	if (terraincollision(m_Lwhisker, hmap))//left whisker colliding
	{
		speed = 5;
		float rotation = (m_node[0].yaw - dt * 100);//Math::Clamp((float)(m_node[0].yaw + dt * 30), (m_node[i - 1].yaw) - 30, (m_node[i - 1].yaw));
		m_node[0].yaw = rotation;
		HandleCollision();
		//std::cout << ""
	}
	if (terraincollision(m_Rwhisker, hmap))//left whisker colliding
	{
		speed = 5;
		float rotation = (m_node[0].yaw + dt * 100);//Math::Clamp((float)(m_node[0].yaw + dt * 30), (m_node[i - 1].yaw) - 30, (m_node[i - 1].yaw));
		m_node[0].yaw = rotation;
		HandleCollision();
	}
	if (terraincollision(m_Rwhisker, hmap) && terraincollision(m_Lwhisker, hmap))//left whisker colliding
	{
		speed = 2;
		float rotation = (m_node[0].yaw + dt * 180);//Math::Clamp((float)(m_node[0].yaw + dt * 30), (m_node[i - 1].yaw) - 30, (m_node[i - 1].yaw));
		m_node[0].yaw = rotation;
		HandleCollision();
	}



	Vector3 direction = Vector3(Math::RadianToDegree(cos(Math::DegreeToRadian(-m_node[0].yaw+90))), vel.y, Math::RadianToDegree(sin(Math::DegreeToRadian(-m_node[0].yaw+90))));//get vel from direction head is facing
	vel = direction.Normalized();//update velocity
	AnimateFrilledShark(dt);
	//pos += vel*dt;
	pos = m_node[0].pos;
	m_node[0].pos += vel*2*dt*speed;


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

		m_node[i].pos.y = m_node[i - 1].pos.y;

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

void FrilledShark::UpdateWhiskers()
{
	float x_com = Math::RadianToDegree(cos(Math::DegreeToRadian(65 - m_node[0].yaw))) * 3.4;
	float z_com = Math::RadianToDegree(sin(Math::DegreeToRadian(65 - m_node[0].yaw))) * 3.4;
	
	float x = pos.x + x_com;
	float z = pos.z + z_com;

	Vector3 position = Vector3(x, m_node[0].pos.y, z);
	hitbox2::updatehitbox(m_Lwhisker, position);

	x_com = Math::RadianToDegree(cos(Math::DegreeToRadian(65 + m_node[0].yaw))) * 3.4;
	z_com = Math::RadianToDegree(sin(Math::DegreeToRadian(65 + m_node[0].yaw))) * 3.4;

	x = pos.x - x_com;
	z = pos.z + z_com;

	position = Vector3(x, m_node[0].pos.y, z);
	hitbox2::updatehitbox(m_Rwhisker, position);
}

void FrilledShark::HandleCollision()
{
	if (m_state != IDLE)
	m_state = STRAFE;
}

void FrilledShark::UpdateHitboxes()
{
	for (unsigned i = 0; i < 5; i++)
	{
		hitbox::updatehitbox(m_FSbox[i],m_node[i].pos);

	}
}