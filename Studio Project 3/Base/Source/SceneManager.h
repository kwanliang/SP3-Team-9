#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H


#include "SceneSP3.h"
#include "SceneTutorial.h"
#include "SceneCalmPlateu.h"
#include "SceneCreepingRidge.h"
#include "SceneGhastlyDepths.h"
#include "SceneNightmareTrench.h"

class SceneManager
{
private:
	SceneSP3 *currentScene;

public:
	SceneManager();
	~SceneManager();

	void LoadScene();
	SceneSP3 *GetCurrentScene();
};

#endif