#ifndef PUFFERFISH_H
#define PUFFERFISH_H

#include "SeaCreature.h"

class Pufferfish : public SeaCreature
{
private:

	
public:
	enum PUFFER_STATE
	{
		IDLE,
		ENRAGED,
		FLEEING,
		STATE_TOTAL,
	};
    Pufferfish();
	Pufferfish(Vector3 pos);
	~Pufferfish();
    void expand();
	PUFFER_STATE pstate;
	

};

#endif