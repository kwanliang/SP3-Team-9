#ifndef CUTTLEFISH_H
#define CUTTLEFISH_H

#include "SeaCreature.h"

class Cuttlefish : public SeaCreature
{
private:

public:
	enum CUTTLE_STATE
	{
		IDLE,
		ATTACKING,
		RETURNING,
		SINK,
		STATE_TOTAL
	};

	Cuttlefish();
	Cuttlefish(Vector3 pos);
	~Cuttlefish();

	void UpdateCuttle(double dt);
	CUTTLE_STATE ctstate;
	float rotate;
	bool jet;

};

#endif