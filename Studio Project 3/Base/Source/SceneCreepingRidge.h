#ifndef SCENE_CREEPINGRIDGE_H
#define SCENE_CREEPINGRIDGE_H

#include "SceneSP3.h"
#include <vector>
#include "Mtx44.h"

static int g_FCrabCount = 0;

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
    void RenderSkyPlane();

private:

    // Shadow

    // Light lights[2];

    //bool bLightEnabled;

    // float fps;

    float theta = 0; //crab jump

    //Fish test

    Capture fishy;
    //Fcrab* c;
};

#endif