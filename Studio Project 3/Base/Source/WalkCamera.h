#pragma once
#include "ThirdPersonCamera.h"

class WalkCamera : public ThirdPersonCamera
{
public:
	WalkCamera();
	~WalkCamera();

	void Init(
		const Vector3 &pos,
		const Vector3 &dir,
		const Vector3 &up,
		const float &distance = 1,
		const Vector3 &vertical = { 0, 1, 0 });

	void Move(const Vector3 &v);
	void Move(const float &frontDist, const float &sideDist = 0, const float &upDist = 0, const bool &followUp = false);
	void Turn(const float &degrees);
	void Pitch(float degrees);

	Mtx44 GetView();

	float yOffset;

protected:
	Vector3 vertical;
};