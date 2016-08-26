#pragma once
#include "Mtx44.h"

class CameraV2
{
public:
	CameraV2() {}
	~CameraV2() {}

	virtual Mtx44 GetProjection() = 0;
	virtual Mtx44 GetView() = 0;
};