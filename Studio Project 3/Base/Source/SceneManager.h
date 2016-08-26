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
    SceneSP3 *previousScene;

public:
	SceneManager();
	~SceneManager();

	void LoadScene();
	SceneSP3 *GetCurrentScene();
    SceneSP3 *GetPreviousScene();

    void SetCurrentScene(SceneSP3* currentScene);
    void SetPreviousScene(SceneSP3* previousScene);

    //SceneTutorial* SceneTutorial;
    //SceneCalmPlateu* SceneCalmPlateu;
    //SceneCreepingRidge* SceneCreepingRidge;
    //SceneGhastlyDepths* SceneGhastlyDepths;
    //SceneNightmareTrench* SceneNightmareTrench;
};

#endif