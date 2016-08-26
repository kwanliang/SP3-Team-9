#ifndef SCENE_TUTORIAL_H
#define SCENE_TUTORIAL_H

#include "SceneSP3.h"
#include <vector>
#include "Mtx44.h"
#include "GiantSquid.h"
#include "Vector2.h"

static float g_distFromGiantSquid = 400;

class SceneTutorial : public SceneSP3
{
public:
	SceneTutorial();
	~SceneTutorial();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Exit();
	
	virtual void Render();
    void RenderBO(Boss *bo);

    void InitSpawner();
    void InitGiantSquid();

	void RenderPassGPass();
	void RenderPassMain();
	void RenderWorld();
	void RenderTerrain();
	void RenderSkyPlane();
	void RenderParticles();

    void UpdateGiantSquid(double dt);

private:

   // Light lights[2];

    bool bLightEnabled;


    // Particles 
    std::vector<ParticleObject*> particleList; // Used to store
    Vector3 m_gravity;      // Gravity affecting the particles
    int m_particleCount;    // Number of particles

	//Fish test
	Capture fishy;

    GiantSquid* giantSquid;

    GiantSquid* tentacle1_1;
    GiantSquid* tentacle1_2;
    GiantSquid* tentacle1_3;
    GiantSquid* tentacle1_4;

    GiantSquid* tentacle2_1;
    GiantSquid* tentacle2_2;
    GiantSquid* tentacle2_3;
    GiantSquid* tentacle2_4;

    GiantSquid* tentacle3_1;
    GiantSquid* tentacle3_2;
    GiantSquid* tentacle3_3;
    GiantSquid* tentacle3_4;

    GiantSquid* tentacle4_1;
    GiantSquid* tentacle4_2;
    GiantSquid* tentacle4_3;
    GiantSquid* tentacle4_4;

    GiantSquid* tentacle5_1;
    GiantSquid* tentacle5_2;
    GiantSquid* tentacle5_3;
    GiantSquid* tentacle5_4;

    GiantSquid* tentacle6_1;
    GiantSquid* tentacle6_2;
    GiantSquid* tentacle6_3;
    GiantSquid* tentacle6_4;
};

#endif