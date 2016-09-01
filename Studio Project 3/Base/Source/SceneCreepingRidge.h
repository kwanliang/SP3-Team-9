#ifndef SCENE_CREEPINGRIDGE_H
#define SCENE_CREEPINGRIDGE_H

#include "SceneSP3.h"
#include <vector>
#include "Mtx44.h"

class SceneCreepingRidge : public SceneSP3
{
public:
    SceneCreepingRidge();
    ~SceneCreepingRidge();

    virtual void Init();
    virtual void Update(double dt);
    virtual void Exit();

    virtual void Render();
    void RenderPassGPass();
    void RenderPassMain();
	void RenderGiantCrab();

    void RenderWorld();
    void RenderTerrain();

private:

    float theta = 0; //crab jump


    Capture fishy;
};

#endif