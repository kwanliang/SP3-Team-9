#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H


#include "SceneSP3.h"
#include "SceneTutorial.h"
#include "SceneCalmPlateu.h"
#include "SceneCreepingRidge.h"
#include "SceneGhastlyDepths.h"
#include "SceneNightmareTrench.h"
#include "SceneMenu.h"
#include "SceneLoading.h"

class SceneManager
{
private:
	SceneSP3 *currentScene;
    SceneSP3 *previousScene;
    SceneSP3 *MenuScreen;
    SceneSP3 *LoadingScreen;

public:
	SceneManager();
	~SceneManager();

	void LoadScene();
	SceneSP3 *GetCurrentScene();
    SceneSP3 *GetPreviousScene();
    SceneSP3 *GetNextScene();
    SceneSP3 *GetMenuScreen();
    SceneSP3 *GetLoadingScreen();

    void SetCurrentScene(SceneSP3* currentScene);
    void SetPreviousScene(SceneSP3* previousScene);
    void SetNextScene(SceneSP3* nextScene);
    void SetMenuScene(SceneSP3* MenuScreen);
    void SetLoadingScene(SceneSP3* LoadingScreen);
};

#endif