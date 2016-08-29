#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector3.h"

enum PARTICLEOBJECT_TYPE
{
    P_PARTICLE,
    P_BUBBLE,
    P_VACUUM,
    P_TOTAL,
};

class ParticleObject
{
public:
    ParticleObject(PARTICLEOBJECT_TYPE);
    ~ParticleObject();

    PARTICLEOBJECT_TYPE type;   // Type of Particle
    Vector3 pos;                // Position of Particle
    Vector3 vel;                // Velocity of Particle
    Vector3 scale;              // Scale of Particle
    float rotation;             // Rotation of Particle
    float rotateSpeed;          // Rotational speed of Particle

    bool active;                // Activate Particle upon use
};

#endif