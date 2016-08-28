#include "SceneTutorial.h"
#include "GL\glew.h"
#include "Application.h"
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
    currentCam = &walkCam;
    walkCam.Init(
        Vector3(0, 400, 0),
        Vector3(0, 0, -10),
        Vector3(0, 1, 0),
        60
        );

    m_travelzonedown = hitbox::generatehitbox(Vector3(52, 579, 1310), 600, 500, 600, 0);
}

void SceneTutorial::RenderTerrain()
{
    modelStack.PushMatrix();
    modelStack.Scale(3000.f, 350.0f, 3000.f);
    RenderMesh(meshList[GEO_TERRAIN0], true);
    modelStack.PopMatrix();
}

void SceneTutorial::RenderSkyPlane()
{
    modelStack.PushMatrix();
    modelStack.Translate(0, 2000, 0);
    modelStack.Rotate(rotateSky, 0, 1, 0);
    RenderMesh(meshList[GEO_SKYPLANE], true);
    modelStack.PopMatrix();
}

void SceneTutorial::RenderParticles()
{
    //for (auto it : particleList)
    //{
    //    ParticleObject* particle = (ParticleObject*)it;
    //    if (particle->active)
    //    {
    //        if (particle->type == PARTICLEOBJECT_TYPE::P_NAME)
    //        {
    //            modelStack.PushMatrix();
    //            modelStack.Translate(particle->pos.x, particle->pos.y, particle->pos.z);
    //            modelStack.Rotate(particle->rotation, 0, 1, 0);
    //            modelStack.Scale(particle->scale.x, particle->scale.y, particle->scale.z);
    //            RenderMesh(meshList[PARTICLE_NAME], false);
    //            modelStack.PopMatrix();
    //        }
    //    }
    //}
}

void SceneTutorial::RenderWorld()
{
    RenderTerrain();
    RenderSkyPlane();
    modelStack.PushMatrix();
    modelStack.Translate(playerpos.x, playerpos.y + 5, playerpos.z);
    modelStack.Rotate(90 + fishRot.y, 0, 1, 0);
    modelStack.Rotate(0 + fishRot.x, 1, 0, 0);
    modelStack.Scale(15, 15, 15);
    RenderMesh(meshList[GEO_FISHMODEL], true);
    modelStack.PushMatrix();
    modelStack.Scale(1, 1, 1);
    modelStack.Translate(0, -0.02, -1.2);
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
    //these matrices define shadows from light position/direction
    //if (lights[0].type == Light::LIGHT_DIRECTIONAL)
    //	m_lightDepthProj.SetToOrtho(-1000, 1000, -1000, 1000, -8000, 8000);
    //else
    //	m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 20);

    //m_lightDepthView.SetToLookAt(lights[0].position.x, lights[0].position.y, lights[0].position.z, 0, 0, 0, 0, 1, 0);


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
    //Mtx44 perspective;
    //perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
    ////perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
    //projectionStack.LoadMatrix(perspective);

    // Camera matrix
    /*viewStack.LoadIdentity();
    viewStack.LookAt(
    camera.position.x, camera.position.y, camera.position.z,
    camera.target.x, camera.target.y, camera.target.z,
    camera.up.x, camera.up.y, camera.up.z
    );*/
    projectionStack.LoadMatrix(currentCam->GetProjection());
    viewStack.LoadMatrix(currentCam->GetView());
    // Model matrix : an identity matrix (model will be at the origin)
    modelStack.LoadIdentity();
    RenderWorld();
    /*if (lights[0].type == Light::LIGHT_DIRECTIONAL)
    {
    Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
    Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
    glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
    }
    else if (lights[0].type == Light::LIGHT_SPOT)
    {
    Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
    glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
    Vector3 spotDirection_cameraspace = viewStack.Top() * lights[0].spotDirection;
    glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
    }
    else
    {
    Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
    glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
    }

    if (lights[1].type == Light::LIGHT_DIRECTIONAL)
    {
    Vector3 lightDir(lights[1].position.x, lights[1].position.y, lights[1].position.z);
    Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
    glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
    }


    if (lights[2].type == Light::LIGHT_SPOT)
    {
    Position lightPosition_cameraspace = viewStack.Top() * lights[2].position;
    glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition_cameraspace.x);
    Vector3 spotDirection_cameraspace = viewStack.Top() * lights[2].spotDirection;
    glUniform3fv(m_parameters[U_LIGHT2_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
    }
    */
    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

    for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
    {
        GameObject *go = (GameObject*)*it;
        if (go->objectType == GameObject::SEACREATURE || go->objectType == GameObject::CAPTURED)
        {
            Minnow *fo = (Minnow*)*it;
            if (fo->active)
            {
                RenderFO(fo);
                //modelStack.PushMatrix();
                //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	//set to line
                //modelStack.Translate(fo->pos.x, fo->pos.y, fo->pos.z);
                //modelStack.Scale(fo->collision.m_width, fo->collision.m_height, fo->collision.m_length);
                //RenderMesh(meshList[GEO_CUBE], false);
                //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	//set back to fill
                //modelStack.PopMatrix();
            }
        }
        else if (go->objectType == GameObject::PROJECTILE)
        {
            Projectile *po = (Projectile*)*it;
            if (po->active)
            {
                RenderPO(po);
            }
        }
    }

    //modelStack.PushMatrix();
    //modelStack.Translate(0, 300, 0);
    //modelStack.Rotate(Math::RadianToDegree(sin(rotater)), 0, 1, 0);
    //modelStack.Scale(50, 50, 50);
    //RenderMesh(meshList[GEO_CUBE], false);
    //modelStack.PopMatrix();

    // Render the crosshair
    glUniform1i(m_parameters[U_IS_GUI], 1);
    RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 10.0f);
    SceneSP3::RenderMinimap();
    RenderMesh(meshList[GEO_AXES], false);
    glUniform1i(m_parameters[U_IS_GUI], 0);
    std::ostringstream ss;
    ss.precision(3);
    ss << "FPS: " << fps;
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 2, 3);
    glUniform1i(m_parameters[U_FOG_ENABLE], 0);
    RenderMinimap();
    glUniform1i(m_parameters[U_FOG_ENABLE], 1);

    modelStack.PushMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	//set to line
    modelStack.Translate(m_travelzonedown.m_position.x, m_travelzonedown.m_position.y, m_travelzonedown.m_position.z);
    modelStack.Scale(m_travelzonedown.m_width, m_travelzonedown.m_height, m_travelzonedown.m_length);
    RenderMesh(meshList[GEO_CUBE], false);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	//set back to fill
    modelStack.PopMatrix();


    modelStack.PushMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	//set to line
    modelStack.Translate(m_travelzoneup.m_position.x, m_travelzoneup.m_position.y, m_travelzoneup.m_position.z);
    modelStack.Scale(m_travelzoneup.m_width, m_travelzoneup.m_height, m_travelzoneup.m_length);
    RenderMesh(meshList[GEO_CUBE], false);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	//set back to fill
    modelStack.PopMatrix();

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