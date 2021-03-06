#ifndef SCENE_CALMPLATEU_H
#define SCENE_CALMPLATEU_H

#include "SceneSP3.h"
#include <vector>
#include "Mtx44.h"
#include "CollisionManager.h"

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

    void RenderGiantSquid();

    void UpdatePuffer(double dt);

    void UpdateGiantSquid(double dt);

    bool isPlayerHit;

private:

    bool bLightEnabled;

    Capture fishy;
};

#endif