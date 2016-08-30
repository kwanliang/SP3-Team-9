#ifndef DRONE_H
#define DRONE_H

#include "SeaCreature.h"

class Drone : public SeaCreature
{
private:

public:
	Drone();
	~Drone();
	enum Dstate
	{
		ATTACK,
		STRAFE,
		COLLIDE,
		STATE_TOTAL,
	};
	
	
	void UpdateDrone(double dt);
	
	hitbox m_hitbox;
	Dstate m_state;

};

#endif