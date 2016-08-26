#include "Pufferfish.h"

Pufferfish::Pufferfish()
{
    this->m_MoveCoolDown = 0.0;
    SeaCreature();
}

Pufferfish::~Pufferfish()
{

}

double Pufferfish::getMoveCoolDown()
{
    return this->m_MoveCoolDown;
}

void Pufferfish::setMoveCoolDown(double m_MoveCoolDown)
{
    this->m_MoveCoolDown = m_MoveCoolDown;
}