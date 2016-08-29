#ifndef SCENELOADING_H
#define SCENELOADING_H

#include <vector>
#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "SharedData.h"
#include "SceneSP3.h"

class SceneLoading : public SceneSP3
{
public:
    SceneLoading();
    ~SceneLoading();

    virtual void Init();
    virtual void Update(double dt);
    virtual void Exit();

    virtual void Render();
};

#endif