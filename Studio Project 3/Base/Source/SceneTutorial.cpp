#include "SceneTutorial.h"
#include "GL\glew.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include <sstream>

using std::cout;
using std::endl;

SceneTutorial::SceneTutorial()
{

}

SceneTutorial::~SceneTutorial()
{

}

void SceneTutorial::Init()
{
    SceneSP3::Init();
    meshList[GEO_TERRAIN0] = MeshBuilder::GenerateTerrain("terrain", "Image//Area0.raw", m_heightMap[0]);

    currentCam = &walkCam;
    walkCam.Init(
        Vector3(0, 400, 0),
        Vector3(0, 0, -10),
        Vector3(0, 1, 0),
        60
        );

    m_travelzonedown = hitbox::generatehitbox(Vector3(52, 579, 1310), 600, 500, 600, 0);
	m_travelzoneup = hitbox::generatehitbox(Vector3(0, -10000, 0), 1, 1, 1, 0);
	
}

void SceneTutorial::RenderTerrain()
{
    modelStack.PushMatrix();
    modelStack.Scale(3000.f, 350.0f, 3000.f);
    RenderMesh(meshList[GEO_TERRAIN0], true);
    modelStack.PopMatrix();
}

void SceneTutorial::RenderWorld()
{
    RenderTerrain();

    SceneSP3::RenderLoop();

    modelStack.PushMatrix();
    modelStack.Translate(playerpos.x, playerpos.y + 5, playerpos.z);
    modelStack.Rotate(90 + fishRot.y, 0, 1, 0);
    modelStack.Rotate(0 + fishRot.x, 1, 0, 0);
    modelStack.Scale(15, 15, 15);
    RenderMesh(meshList[GEO_FISHMODEL], true);
    modelStack.PushMatrix();
    modelStack.Scale(1, 1, 1);
    modelStack.Translate(0, -0.02f, -1.2f);
    modelStack.Rotate(fish_tailrot, 0, 1, 0);

    RenderMesh(meshList[GEO_FISHTAIL], true);
    modelStack.PopMatrix();
    modelStack.PopMatrix();

	SceneSP3::RenderParticles();
}

void SceneTutorial::RenderPassGPass()
{
    m_renderPass = RENDER_PASS_PRE;
    m_lightDepthFBO.BindForWriting();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClear(GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_gPassShaderID);
    RenderWorld();
}

void SceneTutorial::RenderPassMain()
{
    m_renderPass = RENDER_PASS_MAIN;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Application::GetWindowWidth(), Application::GetWindowHeight());

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_programID);

    // Shadow - pass light depth texture
    m_lightDepthFBO.BindForReading(GL_TEXTURE8);

    glUniform1i(m_parameters[U_SHADOW_MAP], 8);
    glUniform1i(m_parameters[U_FOG_ENABLE], 1);

    projectionStack.LoadMatrix(currentCam->GetProjection());
    viewStack.LoadMatrix(currentCam->GetView());
    // Model matrix : an identity matrix (model will be at the origin)
    modelStack.LoadIdentity();
    RenderWorld();
   
    // Render the crosshair

    SceneSP3::RenderParticles();
    glUniform1i(m_parameters[U_FOG_ENABLE], 0);

    SceneSP3::RenderHUD();

    for (std::vector<DamageText *>::iterator it = m_textList.begin(); it != m_textList.end(); ++it)
    {
        DamageText *to = (DamageText*)*it;
        if (to->getActive())
            RenderTO(to);
    }
}


void SceneTutorial::Render()
{
    // PRE RENDER PASS
    RenderPassGPass();

    // MAIN RENDER PASS
    RenderPassMain();
}

void SceneTutorial::Update(double dt)
{
    SceneSP3::Update(dt);
    if (Application::IsKeyPressed('C'))
        std::cout << playerpos << std::endl;
}

void SceneTutorial::Exit()
{
    SceneSP3::Exit();
}