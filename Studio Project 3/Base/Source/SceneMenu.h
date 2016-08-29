#ifndef SCENEMENU_H
#define SCENEMENU_H

#include <vector>
#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "SharedData.h"
#include "SceneSP3.h"

class SceneMenu : public SceneSP3
{
public:
    enum SELECT
    {
        START,
        QUIT,

        NUM_SELECT
    }select;

    SceneMenu();
    ~SceneMenu();

    virtual void Init();
    virtual void Update(double dt);
    virtual void Exit();

    virtual void Render();
};

#endif