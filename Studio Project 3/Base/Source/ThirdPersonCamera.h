#pragma once
#include "CameraV2.h"
#include "Vector3.h"

class ThirdPersonCamera : public CameraV2
{
public:
	ThirdPersonCamera();
	~ThirdPersonCamera();

	virtual void Init(const Vector3 &pos = { 0,0,0 }, const Vector3 &dir = { 0, 0, -1 }, const Vector3 &up = { 0, 1, 0 }, const float &distance = 1);

	Mtx44 GetProjection();
	virtual Mtx44 GetView();
	Vector3 GetPos();
	Vector3 GetDir();
	Vector3 GetUp();
	void SetPos(Vector3);

	float distance;

protected:
	Vector3 pos;
	Vector3 dir; //As a normalized vector.
	Vector3 up; //As a normalized vector.
};