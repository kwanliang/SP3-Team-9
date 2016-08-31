#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include "Vector3.h"
#include <vector>
#include "GameObject.h"

class SharedData
{
public:
    ~SharedData(){};

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

	unsigned SD_CurrentArea = 2;
	bool SD_Down = false;
	Vector3 SD_PlayerPos;
	std::vector<GameObject*> SD_CapturedList;
    bool SD_QuitGame = false;
    bool SD_IsImmobile = false;
    bool SD_BossDead1 = false;
    bool SD_BossDead2 = false;
    bool SD_BossDead3 = false;
    bool SD_BossDead4 = false;
    bool SD_SceneLoaded = false;
	float SD_Lrotate = 0;

private:
    SharedData(){};
};

#endif