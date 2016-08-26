
#ifndef CHIMERA_H
#define CHIMERA_H

#include "SeaCreature.h"

class Chimera : public SeaCreature
{
private:


public:
    struct Flipper
    {
        float rotation = 0;
        bool flipper_max = 0;
    };
    enum CHIMERA_STATE
    {
        IDLE,
        ATTACKING,
        SPEED,
        RETURNING,
        FLEEING,
        STATE_TOTAL,
    };
    Chimera();
    ~Chimera();

    void UpdateChimera(double dt);

    //void expand();
    CHIMERA_STATE chstate;
    float rotate;
    Flipper flip[4];
};

#endif