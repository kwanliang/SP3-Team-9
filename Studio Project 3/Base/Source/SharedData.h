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
		A_TUTORIAL,
		A_CALMPLATAEU,
		A_CREEPINGRIDGE,
		A_GHASTLYDEPTHS,
		A_NIGHTMARETRENCH
	};

	unsigned SD_CurrentArea = 0;
	bool SD_Down = false;
	Vector3 SD_PlayerPos;
    int SD_PlayerHealth = 0;
	std::vector<GameObject*> SD_CapturedList;
    bool SD_QuitGame = false;
    bool SD_IsImmobile = false;
    bool SD_BossDead1 = false;
    bool SD_BossDead2 = false;
    bool SD_BossDead3 = false;
    bool SD_BossDead4 = false;
    bool SD_SceneLoaded = false;
	float SD_Lrotate = 0;
    bool SD_PauseMenu = false;
    bool SD_ContinueVictoryScreen = false;
    bool SD_ContinueCreditsScreen = false;
    bool SD_ToMenu = false;
    bool SD_TurnFPSOn = false;

    //TUTORIAL & OBJECTIVES
    bool SD_DoneTutorial = false;
    bool SD_ContinueInstruction1 = false;
    bool SD_ContinueInstruction2 = false;
    bool SD_ContinueInstruction3 = false;
    bool SD_ObjectiveTabOn = false;

private:
    SharedData(){};
};

#endif