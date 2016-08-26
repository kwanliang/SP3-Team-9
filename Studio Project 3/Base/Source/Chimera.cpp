#include "Chimera.h"
#include "SharedData.h"

Chimera::Chimera()
{
	SeaCreature();
	rotate = Math::RandFloatMinMax(-20, 20);
	chstate = IDLE;
}

Chimera::~Chimera()
{

}

void Chimera::UpdateChimera(double dt)
{
	Vector3 Odisplacement = Vector3(0, 400, 0) - pos ;
	Vector3 Ppos = SharedData::GetInstance()->SD_PlayerPos;
	Vector3 Pdisplacement = Ppos - pos;
	switch (chstate)
	{
	case (IDLE):
	{
		rotate += 15 * dt;
		//std::cout << rotate << std::endl;

		if (Pdisplacement.LengthSquared() < 200 * 200)
			chstate = ATTACKING;

		if (Odisplacement.LengthSquared() > 700 * 700)
			chstate = RETURNING;


		Vector3 direction = Vector3(Math::RadianToDegree(cos(Math::DegreeToRadian(rotate + 90))), Math::Clamp(vel.y, 0.f, 20.f), Math::RadianToDegree(sin(Math::DegreeToRadian(rotate + 90))));
		vel = direction.Normalized() * 80;
	}
		break;

	case (RETURNING) :
	{
		rotate = 180 - (Math::RadianToDegree(atan2(pos.x, pos.z)));

		if (Odisplacement.LengthSquared() < 200 * 200)
		{
			vel.y = Math::RandFloatMinMax(-20, 20);
			rotate = Math::RandFloatMinMax(0, 180);
			chstate = IDLE;

			
		}
		Vector3 direction = Vector3(Math::RadianToDegree(cos(Math::DegreeToRadian(rotate + 90))), Odisplacement.y * 4, Math::RadianToDegree(sin(Math::DegreeToRadian(rotate + 90))));
		vel = direction.Normalized() * 80;
	}
		break;
	case (ATTACKING) :
	{

		rotate = -(Math::RadianToDegree(atan2(Pdisplacement.x, Pdisplacement.z)));

		if (Pdisplacement.LengthSquared() > 400 * 400)
		{
			rotate = Math::RandFloatMinMax(0, 180);
			chstate = IDLE;
		}

		Vector3 direction = Vector3(Math::RadianToDegree(cos(Math::DegreeToRadian(rotate + 90))), Pdisplacement.y * 4, Math::RadianToDegree(sin(Math::DegreeToRadian(rotate + 90))));
		vel = direction.Normalized() * 110;
	}
		break;
	case (FLEEING) :
	{
		//rotate = -rotate;
		if (Pdisplacement.LengthSquared() > 200*200)
			chstate = RETURNING;
		Vector3 direction = Vector3(Math::RadianToDegree(cos(Math::DegreeToRadian(rotate + 90))), 0, Math::RadianToDegree(sin(Math::DegreeToRadian(rotate + 90))));
		vel = direction.Normalized() * 80;
	}
					 break;
	}
	

	if (pos.y > 700)//height limit
	{
		pos.y -= 5;
		vel.y = -vel.y;
	}

	
	

	hitbox::updatehitbox(aabb,pos);

	//animation
	for (unsigned i = 0; i < 4; ++i)
	{
		if (!flip[i].flipper_max)
		{
			if (flip[i].rotation < 40)
				flip[i].rotation += 100 * dt;
			else
				flip[i].flipper_max = true;
		}
		else
		{

			if (flip[i].rotation > -40)
				flip[i].rotation -= 100 * dt;
			else
				flip[i].flipper_max = false;
		}
	}

	
	pos += vel*dt;
}