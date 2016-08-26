#include "ThirdPersonCamera.h"

ThirdPersonCamera::ThirdPersonCamera()
	: pos(0, 0, 0), dir(0, 0, -1), up(0, 1, 0), distance(1)
{

}

ThirdPersonCamera::~ThirdPersonCamera()
{

}

void ThirdPersonCamera::Init(const Vector3 &pos, const Vector3 &dir, const Vector3 &up, const float &distance)
{
	this->pos = pos;
	this->dir = dir;
	this->dir.Normalize();
	this->up = up;
	this->up.Normalize();
	this->distance = distance;
}

Mtx44 ThirdPersonCamera::GetProjection()
{
	Mtx44 proj;
	proj.SetToPerspective(45., 4. / 3., 0.1, 10000.);

	return proj;
}

Mtx44 ThirdPersonCamera::GetView()
{
	Vector3 eye = pos - dir * distance;

	Mtx44 view;
	view.SetToLookAt(
		(double)eye.x, (double)eye.y, (double)eye.z,
		(double)pos.x, (double)pos.y, (double)pos.z,
		(double)up.x, (double)up.y, (double)up.z
	);

	return view;
}

Vector3 ThirdPersonCamera::GetPos()
{
	return pos;
}

void ThirdPersonCamera::SetPos(Vector3 newpos)
{
	pos = newpos;
}


Vector3 ThirdPersonCamera::GetDir()
{
	return dir;
}

Vector3 ThirdPersonCamera::GetUp()
{
	return up;
}