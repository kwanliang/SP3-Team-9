#ifndef SceneNightmareTrench_H
#define SceneNightmareTrench_H

#include "SceneSP3.h"
#include <vector>
#include "Mtx44.h"
#include "Chimera.h"


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
	void RenderMinimap();

	Chimera* FetchChimera();

	void RenderWorld();
	void RenderTerrain();
	void RenderSkyPlane();
	void RenderParticles();
	void RenderChimera(Chimera*);


private:

    // Shadow
	
   // Light lights[2];

    //bool bLightEnabled;

   // float fps;

    //Terrain
    //std::vector<unsigned char> m_heightMap;

    // Particles 
    std::vector<ParticleObject*> particleList; // Used to store
    Vector3 m_gravity;      // Gravity affecting the particles
    int m_particleCount;    // Number of particles
	//Chimera* c;



	//Fish test
	Capture fishy;

};








#endif