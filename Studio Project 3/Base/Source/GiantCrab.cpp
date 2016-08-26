#include "GiantCrab.h"

GiantCrab::GiantCrab()
	: state(GiantCrab::STATE_IDLE), walkAnim(0)
{
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

//void GiantCrab::Move(const double &dist)
//{
//	Vector3 right = direction.Cross(Vector3(0, 1, 0));
//	right.Normalize();
//	pos += right * (float)dist;
//}