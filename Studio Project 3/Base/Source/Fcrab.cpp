#include "Fcrab.h"

Fcrab::Fcrab()
{
	SeaCreature(0, FCRAB, SEACREATURE, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1), false);
	Crableg[1].rotate = 20;
	Crableg[3].rotate = 30;
	Crableg[5].rotate = 20;
	Crableg[0].rotate = -20;
	Crableg[2].rotate = -30;
	Crableg[4].rotate = -20;
}
Fcrab::Fcrab(Vector3 pos)
{
	SeaCreature(500, FCRAB, SEACREATURE, pos, Vector3(0, 0, 0), Vector3(1, 1, 1), false);
	Crableg[1].rotate = 20;
	Crableg[3].rotate = 30;
	Crableg[5].rotate = 20;
	Crableg[0].rotate = -20;
	Crableg[2].rotate = -30;
	Crableg[4].rotate = -20;
	//SeaCreature::m_position = pos;
}
void Fcrab::UpdateFcrab(double dt)
{
	for (unsigned i = 0; i < 6; i++)//animate all legs
	{

			if (!Crableg[i].max)
			{
				if (Crableg[i].rotate < 20)
					Crableg[i].rotate += 100 * dt;
				else
					Crableg[i].max = true;
			}
			else
			{
				if (Crableg[i].rotate > -20)
					Crableg[i].rotate -= 100 * dt;
				else
					Crableg[i].max = false;
			}
	}

	hitbox::updatehitbox(aabb,pos);


	








}
Fcrab::~Fcrab()
{

}

