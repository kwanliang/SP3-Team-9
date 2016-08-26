#include "WalkCamera.h"

const float maxPitch = 80;
const float minPitch = -80;

WalkCamera::WalkCamera()
	: yOffset(0)
{

}

WalkCamera::~WalkCamera()
{

}

void WalkCamera::Init(
	const Vector3 &pos,
	const Vector3 &dir,
	const Vector3 &up,
	const float &distance,
	const Vector3 &vertical)
{
	ThirdPersonCamera::Init(pos, dir, up, distance);
	this->vertical = vertical;
	this->vertical.Normalize();
}

void WalkCamera::Move(const Vector3 &v)
{
	pos += v;
}

void WalkCamera::Move(const float &frontDist, const float &sideDist, const float &upDist, const bool &followUp)
{
	Vector3 right = dir.Cross(up);
	Vector3 front = vertical.Cross(right);
	Vector3 view = pos - dir;
	view.Normalize();
	right.Normalize();
	front.Normalize();

	pos +=
		front * frontDist +
		right * sideDist ;

	if (followUp) pos += up * upDist;
	else pos += vertical * upDist;
}

void WalkCamera::Turn(const float &degrees)
{
	Mtx44 rot;
	rot.SetToRotation(degrees, vertical.x, vertical.y, vertical.z);

	dir = rot * dir;
	up = rot * up;
}

void WalkCamera::Pitch(float degrees)
{
	float angle = acos(dir.Dot(vertical));
	angle = 90.f - Math::RadianToDegree(angle);

	if (degrees > 0)
	{
		if (angle + degrees > maxPitch)  degrees = maxPitch - angle;
	}
	if (degrees < 0)
	{
		if (angle + degrees < minPitch)  degrees = minPitch - angle;
	}

	Vector3 right = dir.Cross(up);
	Mtx44 rot;
	rot.SetToRotation(degrees, right.x, right.y, right.z);

	dir = rot * dir;
	up = rot * up;
}

Mtx44 WalkCamera::GetView()
{
	Vector3 eye = pos - dir * distance + Vector3(0, yOffset, 0);
	Vector3 center = pos + Vector3(0, yOffset, 0);

	Mtx44 view;
	view.SetToLookAt(
		(double)eye.x, (double)eye.y, (double)eye.z,
		(double)center.x, (double)center.y, (double)center.z,
		(double)up.x, (double)up.y, (double)up.z
	);

	return view;
}