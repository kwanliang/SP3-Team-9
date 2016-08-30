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
		RETREAT,
		COLLIDE,
		STATE_TOTAL,
	};


	void UpdateDrone(double dt, std::vector<unsigned char> hmap);
	Vector3 m_location;
	hitbox m_hitbox;
	Dstate m_state;

};

#endif