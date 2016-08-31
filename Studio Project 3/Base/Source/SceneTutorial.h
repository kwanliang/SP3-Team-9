#ifndef SCENE_TUTORIAL_H
#define SCENE_TUTORIAL_H

#include "SceneSP3.h"
#include <vector>
#include "Mtx44.h"
#include "GiantSquid.h"
#include "Vector2.h"

static float g_distFromGiantSquid = 800.f;

class SceneTutorial : public SceneSP3
{
public:
	SceneTutorial();
	~SceneTutorial();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Exit();
	
	virtual void Render();

    void InitSpawner();
    void InitGiantSquid();

	void RenderPassGPass();
	void RenderPassMain();
	void RenderWorld();
	void RenderTerrain();
    void RenderDescription();
    void RenderControls();
    

private:

    // Light lights[2];

    bool bLightEnabled;

    // Particles 
    std::vector<ParticleObject*> particleList; // Used to store
    Vector3 m_gravity;      // Gravity affecting the particles
    int m_particleCount;    // Number of particles

    //Fish test
    Capture fishy;
};

#endif