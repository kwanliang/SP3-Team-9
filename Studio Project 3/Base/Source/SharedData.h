#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include "Vector3.h"

class SharedData
{
public:
    static SharedData* GetInstance()
    {
        static SharedData data;
        return &data;
    }

	enum AREA
	{
		A_TUTORIAL,
		A_CALMPLATAEU,
		A_CREEPINGRIDGE,
		A_GHASTLYDEPTHS,
		A_NIGHTMARETRENCH
	};

	unsigned SD_CurrentArea = 1;
	bool SD_Down = false;
	Vector3 SD_PlayerPos;

private:
    SharedData(){};
};

#endif