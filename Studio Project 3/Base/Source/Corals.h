#ifndef CORALS_H
#define CORALS_H

#include "GameObject.h"

class Coral : public GameObject
{
private:

public:
    enum CORAL_TYPE
    {
        HEALTH,
        STAMINA,

        TYPE_TOTAL,
    };
    CORAL_TYPE coralType;

    Coral();
    ~Coral();

};

#endif