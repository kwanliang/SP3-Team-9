#ifndef CAPTURE_H_
#define CAPTURE_H_

#include "Camera3.h"
#include "WalkCamera.h"
#include "SeaCreature.h"
#include "Corals.h"

#include <vector>

class Capture
{
public:
	Capture();
	~Capture();

	static Vector3 Vacuum(SeaCreature creature, Vector3 playerpos, bool check);
	static GameObject::OBJECT_TYPE AddSquad(SeaCreature creature, Vector3 playerpos, bool check);
	static bool rangeCheckXZ(WalkCamera camera, SeaCreature creature, Vector3 playerpos);
	static bool rangeCheckY(WalkCamera camera, SeaCreature creature, Vector3 playerpos);
	
	static int GetSquadSize();
private:
	static float range;
	
};

#endif