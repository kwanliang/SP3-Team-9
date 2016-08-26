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

}

SceneSP3 *SceneManager::GetCurrentScene()
{
	return this->currentScene;
}