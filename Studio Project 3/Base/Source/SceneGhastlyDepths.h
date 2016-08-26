#ifndef SCENEGHASTLYDEPTHS_H
#define SCENEGHASTLYDEPTHS_H

#include "SceneSP3.h"
#include <vector>
#include "Mtx44.h"
#include "FrilledShark.h"
#include "Cuttlefish.h"

class SceneGhastlyDepths : public SceneSP3
{
public:
	SceneGhastlyDepths();
	~SceneGhastlyDepths();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Exit();
	
	virtual void Render();
	void RenderPassGPass();
	void RenderPassMain();
	void RenderMinimap();

	Cuttlefish* FetchCuttle();
	void RenderBoss();
	void RenderCuttle(Cuttlefish* c);
	void RenderWorld();
	void RenderTerrain();
	void RenderSkyPlane();

private:

    // Shadow
	
   // Light lights[2];

    //bool bLightEnabled;

   // float fps;

    //Terrain
    //std::vector<unsigned char> m_heightMap;

	FrilledShark* frilledshark;

	//Fish test
	Capture fishy;

	//Cuttlefish* c;

};

#endif