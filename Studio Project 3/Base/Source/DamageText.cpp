#include "DamageText.h"

DamageText::DamageText()
    : m_LastHitPos(Vector3(0, 0, 0))
    , m_LastDamage(0)
    , m_ScaleText(Vector3(0, 0, 0))
    , m_isActive(false)
{

}

DamageText::~DamageText()
{

}

Vector3 DamageText::getLastHitPos()
{
    return this->m_LastHitPos;
}

void DamageText::setLastHitPos(Vector3 m_LastHitPos)
{
    this->m_LastHitPos = m_LastHitPos;
}

int DamageText::getLastDamage()
{
    return this->m_LastDamage;
}

void DamageText::setLastDamage(int m_LastDamage)
{
    this->m_LastDamage = m_LastDamage;
}

Vector3 DamageText::getScaleText()
{
    return this->m_ScaleText;
}

void DamageText::setScaleText(Vector3 m_ScaleText)
{
    this->m_ScaleText = m_ScaleText;
}

bool DamageText::getActive()
{
    return this->m_isActive;
}

void DamageText::setActive(bool m_isActive)
{
    this->m_isActive = m_isActive;
}