#include "Capture.h"
#include <math.h>
#include "MyMath.h"


Capture::Capture()
{
}


Capture::~Capture()
{
}

Vector3 Capture::Vacuum(SeaCreature creature, Vector3 playerpos, bool check)
{
	if (check == true && creature.getHealth() <= 50)
	{
		float distance = fabs(playerpos.x - creature.pos.x) + fabs(playerpos.y - creature.pos.y) + fabs(playerpos.z - creature.pos.z);
		Vector3 view = (creature.pos - playerpos).Normalized();
		if (fabs(creature.pos.x - playerpos.x) >= 1 && distance >= 1)
		{
			creature.pos.Set((creature.pos.x - view.x  * 50 / distance), (creature.pos.y), (creature.pos.z));
		}
		if (fabs(creature.pos.y - playerpos.y) >= 1 && distance >= 1)
		{
			creature.pos.Set(creature.pos.x, (creature.pos.y - view.y  * 50 / distance), (creature.pos.z));
		}
		if (fabs(creature.pos.z - playerpos.z) >= 1 && distance >= 1)
		{
			creature.pos.Set(creature.pos.x, (creature.pos.y), (creature.pos.z - view.z  * 50 / distance));
		}
	}
	return creature.pos;
}

GameObject::OBJECT_TYPE Capture::AddSquad(SeaCreature creature, hitbox2 playerbox, bool check)
{
	//float dist = fabs(playerpos.x - creature.pos.x) + fabs(playerpos.y - creature.pos.y) + fabs(playerpos.z - creature.pos.z);
	if (collision(creature.collision, playerbox))
	{
		return GameObject::CAPTURED;
	}
	else
	{
		return creature.objectType;
	}

}

bool Capture::rangeCheckXZ(WalkCamera camera, SeaCreature creature, Vector3 playerpos)
{
	float dist = fabs(playerpos.x - creature.pos.x) + fabs(playerpos.y - creature.pos.y) + fabs(playerpos.z - creature.pos.z);
	float angle = Math::RadianToDegree(atan2(camera.GetDir().x, camera.GetDir().z));
	float playertocreature = Math::RadianToDegree(atan2((creature.pos.x - playerpos.x), (creature.pos.z - playerpos.z)));

	float range1 = angle + 30;
	float range2 = angle - 30;
	if (range1 > 180)
	{
		range1 -= 360;
	}
	else if (range2 < -180)
	{
		range2 += 360;
	}

	if (fabs(range1 - range2)> 70)
	{
		if (dist <= 250 && (range1 < playertocreature && range2 < playertocreature) || (range1 > playertocreature && range2 > playertocreature))
		{
			return rangeCheckY(camera, creature, playerpos);
		}
	}
	else
	{
		if (dist <= 250 && range1 > playertocreature && range2 < playertocreature)
		{
			return rangeCheckY(camera, creature, playerpos);
		}
	}
	return false;
}

bool Capture::rangeCheckY(WalkCamera camera, SeaCreature creature, Vector3 playerpos)
{
	float dist = (fabs(playerpos.x - creature.pos.x) + fabs(playerpos.y - creature.pos.y) + fabs(playerpos.z - creature.pos.z));
	float angle = Math::RadianToDegree(atan(camera.GetDir().y));
	Vector3 target = (creature.pos - playerpos).Normalized();
	float playertocreature = Math::RadianToDegree(atan(target.y));
	

	float range1 = angle + 30;
	float range2 = angle - 30;
	if (range1 > 180)
	{
		range1 -= 360;
	}
	else if (range2 < -180)
	{
		range2 += 360;
	}
	
	if (fabs(range1 - range2)> 70)
	{
		if (dist <= 250 && (range1 < playertocreature && range2 < playertocreature) || (range1 > playertocreature && range2 > playertocreature))
		{
			return true;
		}
	}
	else
	{
		if (dist <= 250 && range1 > playertocreature && range2 < playertocreature)
		{
			return true;
		}
	}
}