#include "Fcrab.h"

Fcrab::Fcrab()
{
	SeaCreature();
	Crableg[1].rotate = 20;
	Crableg[3].rotate = 30;
	Crableg[5].rotate = 20;
	Crableg[0].rotate = -20;
	Crableg[2].rotate = -30;
	Crableg[4].rotate = -20;
}

Fcrab::~Fcrab()
{

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

    hitbox::updatehitbox(aabb, pos);

}
