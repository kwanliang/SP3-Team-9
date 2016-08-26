#ifndef SCENE_CREEPINGRIDGE_H
#define SCENE_CREEPINGRIDGE_H

#include "SceneSP3.h"
#include <vector>
#include "Mtx44.h"
#include "Fcrab.h"
#include "GiantCrab.h"

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
    void RenderMinimap();
	void RenderGiantCrab();

    void RenderWorld();
    void RenderTerrain();
    void RenderSkyPlane();
    void RenderFcrab(Fcrab* c);

    Fcrab* FetchFcrab();

private:

    // Shadow

    // Light lights[2];

    //bool bLightEnabled;

    // float fps;

    float theta = 0; //crab jump



    //Fish test
	GiantCrab* giantCrab;
    Capture fishy;
    //Fcrab* c;
};

#endif