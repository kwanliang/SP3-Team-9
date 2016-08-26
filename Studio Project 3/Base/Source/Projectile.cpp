#include "Projectile.h"

Projectile::Projectile()
    : m_lifetime(0.0)
    , projectileType(BULLET)
    , GameObject()
{

}


Projectile::~Projectile()
{

}

double Projectile::getLifetime()
{
    return this->m_lifetime;
}

void Projectile::setLifetime(double m_lifetime)
{
    this->m_lifetime = m_lifetime;
}