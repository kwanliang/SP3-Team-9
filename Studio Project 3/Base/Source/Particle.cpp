#include "Particle.h"

ParticleObject::ParticleObject(PARTICLEOBJECT_TYPE type) :
type(type),
pos(0, 0, 0),
scale(1, 1, 1),
rotation(0),
rotateSpeed(0),
active(false)
{
}

ParticleObject::~ParticleObject()
{
}

