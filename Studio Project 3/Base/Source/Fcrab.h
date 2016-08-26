#ifndef FCRAB_H
#define FCRAB_H

#include "SeaCreature.h"

class Fcrab : public SeaCreature
{
private:


public:
	struct Crabpart
	{
		//Vector3 pos;
		float rotate = 0;
		bool max;

	};
	enum FCRAB_STATE
	{
		IDLE,
		ATTACKING,
		STATE_TOTAL,
	};

	float yaw;
	Fcrab();
	Fcrab(Vector3 pos);
	void UpdateFcrab(double);
	~Fcrab();
	FCRAB_STATE  FCstate;
	Crabpart Crableg[6];
	Crabpart Crabclaw;

};

#endif