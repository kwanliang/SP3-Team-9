#include "Cuttlefish.h"

Cuttlefish::Cuttlefish()
{
	SeaCreature(0, CUTTLE, SEACREATURE, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1), false);
	rotate = 0;
	ctstate = IDLE;
	jet = false;
}

Cuttlefish::Cuttlefish(Vector3 pos)
{
	SeaCreature(0, CUTTLE, SEACREATURE, pos, Vector3(0, 0, 0), Vector3(1, 1, 1), false);
	rotate = 0;
	ctstate = IDLE;
	jet = false;
	
}

Cuttlefish::~Cuttlefish()
{

}

void Cuttlefish::UpdateCuttle(double dt)
{
	switch (ctstate)
	{
	case (IDLE) :
	{
		if (!jet)
		{
			if (vel.Length() > 1)
			{
				vel -= vel.Normalized()*dt * 35;

				//std::cout << decel << std::endl;
			}
			else
				jet = true;
		}
		else if (jet)
		{
			vel = Vector3(Math::RandFloatMinMax(-50, 50), Math::RandFloatMinMax(-20, 20), Math::RandFloatMinMax(-50, 50));
			jet = false;
		}

		rotate = Math::RadianToDegree(atan2(vel.x, vel.z));
		
	}break;

	case (SINK) :
	{
		
		if (pos.y < 400)
			ctstate = IDLE;
		
	}break;
	}
	
	
	if (pos.y > 700)
	{
		vel = Vector3(Math::RandFloatMinMax(-60, 50), Math::RandFloatMinMax(-20, -8), Math::RandFloatMinMax(-30, 60));
		ctstate = SINK;
	}
	
	
	hitbox2::updatehitbox(collision,pos);
	pos += vel*dt*2;
}