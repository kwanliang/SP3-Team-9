#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include "Vector3.h"
#include <vector>
#include "GameObject.h"

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
        //A_MENU,
		A_TUTORIAL,
		A_CALMPLATAEU,
		A_CREEPINGRIDGE,
		A_GHASTLYDEPTHS,
		A_NIGHTMARETRENCH
	};

	unsigned SD_CurrentArea = 4;
	bool SD_Down = false;
	Vector3 SD_PlayerPos;
	std::vector<GameObject*> SD_CapturedList;
    bool SD_ToMenu = false;
    bool SD_QuitGame = false;
    bool SD_IsImmobile = false;

private:
    SharedData(){};
};

#endif