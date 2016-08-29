#ifndef SceneNightmareTrench_H
#define SceneNightmareTrench_H

#include "SceneSP3.h"
#include <vector>
#include "Mtx44.h"

static int g_ChimeraCount = 0;
static int g_IsopodDroneCount = 0;

class SceneNightmareTrench : public SceneSP3
{
public:
	SceneNightmareTrench();
	~SceneNightmareTrench();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Exit();
	
	virtual void Render();
	void RenderPassGPass();
	void RenderPassMain();

	void RenderWorld();
	void RenderTerrain();
	void RenderSkyPlane();

    void RenderBoss();

private:

    // Shadow
	
   // Light lights[2];

    //bool bLightEnabled;

   // float fps;

    //Terrain
    //std::vector<unsigned char> m_heightMap;


	//Fish test
	Capture fishy;
};

#endif