#ifndef DRONE_H
#define DRONE_H

#include "SeaCreature.h"

class Drone : public SeaCreature
{
private:

public:
	Drone();
	~Drone();
	void UpdateDrone(double dt);
};

#endif