#include "SceneManager.h"

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{

}


void SceneManager::LoadScene()
{
	switch (SharedData::GetInstance()->SD_CurrentArea)
	{
	case SharedData::A_TUTORIAL:
		currentScene = new SceneTutorial();
		currentScene->Init();
		break;

	case SharedData::A_CALMPLATAEU:
		currentScene = new SceneCalmPlateu();
		currentScene->Init();
		break;

	case SharedData::A_CREEPINGRIDGE:
        currentScene = new SceneCreepingRidge();
        currentScene->Init();
		break;

	case SharedData::A_GHASTLYDEPTHS :
		currentScene = new SceneGhastlyDepths();
		currentScene->Init();
		break;

	case SharedData::A_NIGHTMARETRENCH:
		currentScene = new SceneNightmareTrench();
		currentScene->Init();
		break;
	}

    SharedData::GetInstance()->SD_SceneLoaded = true;
}

SceneSP3 *SceneManager::GetCurrentScene()
{
	return this->currentScene;
}

SceneSP3 *SceneManager::GetPreviousScene()
{
    return this->previousScene;
}

void SceneManager::SetCurrentScene(SceneSP3* currentScene)
{
    this->currentScene = currentScene;
}

void SceneManager::SetPreviousScene(SceneSP3* previousScene)
{
    this->previousScene = previousScene;
}

SceneSP3 *SceneManager::GetMenuScreen()
{
    return this->MenuScreen;
}

void SceneManager::SetMenuScene(SceneSP3* MenuScreen)
{
    this->MenuScreen = MenuScreen;
}

SceneSP3 *SceneManager::GetLoadingScreen()
{
    return this->LoadingScreen;
}

void SceneManager::SetLoadingScene(SceneSP3* LoadingScreen)
{
    this->LoadingScreen = LoadingScreen;
}