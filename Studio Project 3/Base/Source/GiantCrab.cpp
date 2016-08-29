#include "GiantCrab.h"
#include "SharedData.h"

GiantCrab::GiantCrab()
	: state(GiantCrab::IDLE), walkAnim(0), m_rotate (0)
{
	//float a = 200;
	//float b = 100;

	//speed = 10;
	m_leg[0].Ulegmax = true;
	m_leg[0].Llegmax = true;
	m_leg[0].m_Urotate = 10;
	m_leg[0].m_Lrotate = 12;


	m_leg[1].Ulegmax = false;
	m_leg[1].Llegmax =false;
	m_leg[1].m_Urotate = 4;
	m_leg[1].m_Lrotate = 8;

	m_leg[2].Ulegmax = true;
	m_leg[2].Llegmax = true;
	m_leg[2].m_Urotate = 19;
	m_leg[2].m_Lrotate = -13;

	m_leg[3].Ulegmax = false;
	m_leg[3].Llegmax = false;
	m_leg[3].m_Urotate = -4;
	m_leg[3].m_Lrotate = -21;

	//////////////////////

	m_leg[4].Ulegmax = false;
	m_leg[4].Llegmax = false;
	m_leg[4].m_Urotate = 3;
	m_leg[4].m_Lrotate = -12;


	m_leg[5].Ulegmax = true;
	m_leg[5].Llegmax = true;
	m_leg[5].m_Urotate = -8;
	m_leg[5].m_Lrotate = 19;

	m_leg[6].Ulegmax = false;
	m_leg[6].Llegmax = false;
	m_leg[6].m_Urotate = 11;
	m_leg[6].m_Lrotate = 29;

	m_leg[7].Ulegmax = true;
	m_leg[7].Llegmax = true;
	m_leg[7].m_Urotate = 17;
	m_leg[7].m_Lrotate = 34;


	grabArea = hitbox::generatehitbox(Vector3(0, 0, 0), 120, 30, 120, NULL);

}

GiantCrab::~GiantCrab()
{

}

GiantCrab::CrabState GiantCrab::GetState() const
{
	return state;
}

void GiantCrab::SetState(const GiantCrab::CrabState &state)
{
	this->state = state;
}

float GiantCrab::GetWalkAnim(const unsigned &id)
{
	switch (id)
	{
	case 0:
		return 20 * sin(walkAnim) + 15;
		break;
	case 1:
		return -20 * cos(walkAnim) + 15;
		break;
	}

	return 0.f;
}

void GiantCrab::Move(const double &dist)
{
	//Vector3 right = direction.Cross(Vector3(0, 1, 0));
	//right.Normalize();
	//pos += right * (float)dist;
}

void GiantCrab::UpdateGC(double dt, std::vector<unsigned char> hmap)
{
	float h = 350.f * ReadHeightMap(hmap, pos.x / 3000.f, pos.z / 3000.f) + 170;
	Vector3 P_pos = SharedData::GetInstance()->SD_PlayerPos;
	Vector3 P_displacement = P_pos  -pos;
	//std::cout << Pdisplacement.Length() << std::endl;
	
	if (pos.y < h)
		pos.y += dt * 10;
	else if (pos.y > h)
		pos.y -= dt * 10;

	switch(state)
	{
	case IDLE:
		speed = 6;
		if (P_displacement.LengthSquared() < 600 * 600)
		{
			state = AGGRO;
		}

		break;
	case AGGRO:
		speed = 15;
		vel = P_displacement.Normalized();
		m_rotate = Math::RadianToDegree(atan2(P_displacement.x, P_displacement.z))-90;
		//need collision here
		//if player enters grabbox, grab mode

		if (P_displacement.Length() > 500)
		{
			state = VORTEX;
		}

		break;
	case GRAB:
		speed = 0;
		//vel.Normalize();
		//if crab takes damage, release --> strafe 


		break;
	case VORTEX:
		speed = 1;
		vel = P_pos - grabArea.m_position;
		vel.Normalize();



		break;
	}

	vel.y = 0;
	pos += vel*dt*speed;

	UpdateArms(dt);
	AnimateGC(dt);
	
}

void GiantCrab::UpdateArms(double dt)
{







	Vector3 P_pos = SharedData::GetInstance()->SD_PlayerPos;
	Vector3 Pdisplacement = pos - P_pos;
	float x_com, z_com, y_com;
	float x, y, z;

	//grabbox
	x_com = Math::RadianToDegree(cos(Math::DegreeToRadian(-m_rotate))) * 2.5;
	z_com = Math::RadianToDegree(sin(Math::DegreeToRadian(-m_rotate))) * 2.5;

	hitbox::updatehitbox(grabArea, Vector3(pos.x + x_com, pos.y, pos.z + z_com));


	//left
	x_com = Math::RadianToDegree(cos(Math::DegreeToRadian(65 - m_rotate))) * 0.9;
	z_com = Math::RadianToDegree(sin(Math::DegreeToRadian(65 - m_rotate))) * 0.9;

	x = pos.x + x_com;
	z = pos.z + z_com;
	m_Larm.m_Upos = Vector3(x, pos.y, z);


	//right
	x_com = Math::RadianToDegree(cos(Math::DegreeToRadian(-65 - m_rotate))) * 0.9;
	z_com = Math::RadianToDegree(sin(Math::DegreeToRadian(-65 - m_rotate))) * 0.9;

	x = pos.x + x_com;
	z = pos.z + z_com;
	m_Rarm.m_Upos = Vector3(x, pos.y, z);

	//m_Larm.y_upper m_rotate;


	switch (state)
	{
	case IDLE:
		m_Larm.y_upper = -50;
		m_Larm.x_upper = 55;
		m_Larm.x_lower = -120;

		m_Rarm.y_upper = 50;
		m_Rarm.x_upper = 55;
		m_Rarm.x_lower = -120;

	break;

	case AGGRO:
		m_Larm.y_upper = -50;
		m_Larm.x_upper = 75;
		m_Larm.x_lower = -120;

		m_Rarm.y_upper = 50;
		m_Rarm.x_upper = 75;
		m_Rarm.x_lower = -120;

		break;

	case VORTEX:
		m_Larm.y_upper = -80;
		m_Larm.x_upper = 75;
		m_Larm.x_lower = -120;

		m_Rarm.y_upper = 80;
		m_Rarm.x_upper = 75;
		m_Rarm.x_lower = -120;

		break;

	case GRAB:

		Vector3 Ldisp = m_Larm.m_Upos - P_pos;
		float c = Ldisp.Length();
		float upperY_angle, upperX_angle, lowerX_angle;
		upperY_angle = Math::RadianToDegree(atan2(Ldisp.x, Ldisp.z));//upper arm y rotation
		m_Larm.y_upper = upperY_angle + 90 - m_rotate;
		upperX_angle = Math::RadianToDegree(acos((((c*c) + (a*a) - (b*b)) / (2 * (c*a)))));
		m_Larm.x_upper = upperX_angle;
		lowerX_angle = Math::RadianToDegree(acos(-((a*a) + (b*b) - (c*c)) / (2 * (a*b))));
		m_Larm.x_lower = -lowerX_angle;



		Vector3 Rdisp = m_Rarm.m_Upos - P_pos;
		upperY_angle = Math::RadianToDegree(atan2(Rdisp.x, Rdisp.z));//upper arm y rotation
		m_Rarm.y_upper = upperY_angle + 90 - m_rotate;
		upperX_angle = Math::RadianToDegree(acos((((c*c) + (a*a) - (b*b)) / (2 * (c*a)))));
		m_Rarm.x_upper = upperX_angle;
		lowerX_angle = Math::RadianToDegree(acos(-((a*a) + (b*b) - (c*c)) / (2 * (a*b))));
		m_Rarm.x_lower = -lowerX_angle;
		break;
	}

	

}

void GiantCrab::AnimateGC(double dt)
{


	for (unsigned i = 0; i < 8; ++i)
	{
		if (m_leg[i].Ulegmax && m_leg[i].Llegmax)//upper
		{
			if (m_leg[i].m_Urotate > -10)
			{
                m_leg[i].m_Urotate -= speed * (float)dt;
			}
			else
			{
				m_leg[i].Ulegmax = false;
			}

		}
		else if ((!m_leg[i].Ulegmax && !m_leg[i].Llegmax))
		{
			if (m_leg[i].m_Urotate < 8)
			{
                m_leg[i].m_Urotate += speed * (float)dt;
			}
			else
			{
				m_leg[i].Ulegmax = true;
			}
		}


		if (m_leg[i].Llegmax )
		{
			if (m_leg[i].m_Lrotate < 19)
			{
                m_leg[i].m_Lrotate += speed * (float)dt;
			}
			else
			{
				m_leg[i].Llegmax = false;
			}

		}
		else
		{
			if (m_leg[i].m_Lrotate > -15)
			{
				m_leg[i].m_Lrotate -= speed * (float)dt;
			}
			else
			{
				m_leg[i].Llegmax = true;
			}
		}





	}

}