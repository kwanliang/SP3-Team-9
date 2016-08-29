#include "Drone.h"
#include "SharedData.h"

Drone::Drone()
{
	SeaCreature();
}

Drone::~Drone()
{

}

void Drone::UpdateDrone(double dt)
{
	Vector3 Ppos = SharedData::GetInstance()->SD_PlayerPos;
	Vector3 P_displacement = Ppos - pos;
	vel = P_displacement.Normalized();
	//hitbox2::updatehitbox(collision, pos);
	pos += vel*dt * 8;
}