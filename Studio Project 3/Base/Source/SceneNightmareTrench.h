#ifndef SceneNightmareTrench_H
#define SceneNightmareTrench_H

#include "SceneSP3.h"
#include <vector>
#include "Mtx44.h"

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

    void RenderBoss();

private:


	//Fish test
	Capture fishy;
};

#endif