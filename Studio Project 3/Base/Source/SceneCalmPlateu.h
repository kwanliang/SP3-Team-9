#ifndef SCENE_CALMPLATEU_H
#define SCENE_CALMPLATEU_H

#include "SceneSP3.h"
#include <vector>
#include "Mtx44.h"
#include "CollisionManager.h"
#include "GiantSquid.h"
#include "Pufferfish.h"

static int g_PufferfishCount = 0;

class SceneCalmPlateu : public SceneSP3
{
public:
    SceneCalmPlateu();
    ~SceneCalmPlateu();

    virtual void Init();
    virtual void Update(double dt);
    virtual void Exit();

    void InitGiantSquid();

    virtual void Render();
    void RenderPassGPass();
    void RenderPassMain();

    void RenderWorld();
    void RenderTerrain();
    void RenderSkyPlane();
    void RenderPuffer(Pufferfish *fo);

    Pufferfish* FetchPuffer();

    void RenderGiantSquid();

    void UpdatePuffer(double dt);

    void UpdateGiantSquid(double dt);

    bool isPlayerHit;

private:

    // Shadow

    // Light lights[2];

    bool bLightEnabled;

    // float fps;

    //Terrain
    //std::vector<unsigned char> m_heightMap;

    //Fish test
    Capture fishy;

    GiantSquid* giantSquid;

    Pufferfish* pf;
};

#endif