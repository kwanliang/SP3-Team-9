#ifndef PUFFERFISH_H
#define PUFFERFISH_H

#include "SeaCreature.h"
#include "SharedData.h"

class Pufferfish : public SeaCreature
{
protected:
    double m_MoveCoolDown;

public:
    enum PUFFER_STATE
    {
        IDLE,
        ENRAGED,
        FLEEING,
        STATE_TOTAL,
    };
    PUFFER_STATE pstate;

    Pufferfish();
    ~Pufferfish();

    double getMoveCoolDown();
    void setMoveCoolDown(double m_MoveCoolDown);
};

#endif