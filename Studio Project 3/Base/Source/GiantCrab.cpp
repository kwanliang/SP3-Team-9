#include "GiantCrab.h"
#include "SharedData.h"

GiantCrab::GiantCrab()
	: state(GiantCrab::IDLE), walkAnim(0)
{
	speed = 10;
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

void GiantCrab::updateGC(double dt)
{
	Vector3 P_pos = SharedData::GetInstance()->SD_PlayerPos;
	Vector3 Pdisplacement = pos - P_pos;
	//std::cout << Pdisplacement.Length() << std::endl;

	if (Pdisplacement.LengthSquared() > 500 * 500)
	{
		float upperY_angle = Math::RadianToDegree(atan2(Pdisplacement.x, Pdisplacement.z));//upper arm y rotation
		m_Larm.y_upper = upperY_angle + 90;

		float vertical_displace = Pdisplacement.y;
		float lateral_displace = sqrt((Pdisplacement.x*Pdisplacement.x) + (Pdisplacement.z*Pdisplacement.z));
		float  upperX_angle = -Math::RadianToDegree(atan2(vertical_displace, lateral_displace));//upper arm x rotation
		m_Larm.x_upper = upperX_angle;
	}
	else
	{
		float upperY_angle = Math::RadianToDegree(atan2(Pdisplacement.x, Pdisplacement.z));//upper arm y rotation
		m_Larm.y_upper = upperY_angle + 90+20;
		
		//init sides of triangle
		float c = Pdisplacement.Length()-150;
		std::cout << c << std::endl;
		float a = 180;
		float b = 100;

		//float  upperX_angle = Math::RadianToDegree(acos(Math::DegreeToRadian (((b*b) + (c*c) - (a*a)) / (2 * (b*c)) ) ));
		////std::cout << upperX_angle << std::endl;
		//m_Larm.x_upper = upperX_angle;

		float  upperX_angle = Math::RadianToDegree(acos((((c*c) + (a*a) - (b*b)) / (2 * (c*a)))));
		//std::cout << upperX_angle << std::endl;
		m_Larm.x_upper = upperX_angle;
		
		float lowerX_angle = Math::RadianToDegree( acos( -((a*a) + (b*b) - (c*c)) / (2 * (a*b))) );
		//std::cout << lowerX_angle << std::endl;
		m_Larm.x_lower = lowerX_angle;
	}



	AnimateGC(dt);
	
}


void GiantCrab::AnimateGC(double dt)
{

	for (unsigned i = 0; i < 8; ++i)
	{
		if (m_leg[i].Ulegmax && m_leg[i].Llegmax)//upper
		{
			if (m_leg[i].m_Urotate > -10)
			{
				m_leg[i].m_Urotate -= speed * dt;
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
				m_leg[i].m_Urotate += speed * dt;
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
				m_leg[i].m_Lrotate += speed * dt;
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
				m_leg[i].m_Lrotate -= speed * dt;
			}
			else
			{
				m_leg[i].Llegmax = true;
			}
		}





	}

}