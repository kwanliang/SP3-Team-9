#include "Pufferfish.h"

Pufferfish::Pufferfish()
{
	SeaCreature(0, PUFFER, SEACREATURE, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1), false);
}
Pufferfish::Pufferfish(Vector3 pos)
{
	SeaCreature(40, PUFFER, SEACREATURE, pos, Vector3(0, 0, 0), Vector3(1, 1, 1), false);
	//SeaCreature::m_position = pos;
}
Pufferfish::~Pufferfish()
{

}

void Pufferfish::expand()
{

}