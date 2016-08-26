#ifndef DAMAGETEXT_H
#define DAMAGETEXT_H

static float TextScaleValue = 10.f;

#include "Vector3.h"

class DamageText
{
private:
    Vector3 m_LastHitPos;
    int m_LastDamage;
    Vector3 m_ScaleText;
    bool m_isActive;

public:
    DamageText();
    ~DamageText();

    Vector3 getLastHitPos();
    void setLastHitPos(Vector3 m_LastHitPos);

    int getLastDamage();
    void setLastDamage(int m_LastDamage);

    Vector3 getScaleText();
    void setScaleText(Vector3 m_ScaleText);

    bool getActive();
    void setActive(bool m_isActive);
};

#endif