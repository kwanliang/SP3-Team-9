#include "SceneCreepingRidge.h"
#include "GL\glew.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include <sstream>

using std::cout;
using std::endl;

SceneCreepingRidge::SceneCreepingRidge()
{

}

SceneCreepingRidge::~SceneCreepingRidge()
{

}

void SceneCreepingRidge::Init()
{
    SceneSP3::Init();
    glClearColor(0.1f, 0.5f, 0.5f, 0.0f);
    Color fogColor(0.1f, 0.5f, 0.5f);
    glUniform3fv(m_parameters[U_FOG_COLOR], 1, &fogColor.r);

    meshList[GEO_TERRAIN2] = MeshBuilder::GenerateTerrain("terrain", "Image//Area02.raw", m_heightMap[2]);

    meshList[GEO_CRAB_BODY] = MeshBuilder::GenerateOBJ("crabBody", "Models//OBJ//scrab_body.obj");
    meshList[GEO_CRAB_BODY]->textureArray[0] = LoadTGA("Image//scrab.tga");
    meshList[GEO_CRAB_LEG_UPPER] = MeshBuilder::GenerateOBJ("crabLegUpper", "Models//OBJ//scrab_Uleg.obj");
    meshList[GEO_CRAB_LEG_UPPER]->textureArray[0] = LoadTGA("Image//scrab.tga");
    meshList[GEO_CRAB_LEG_LOWER] = MeshBuilder::GenerateOBJ("crabLegLower", "Models//OBJ//scrab_Lleg.obj");
    meshList[GEO_CRAB_LEG_LOWER]->textureArray[0] = LoadTGA("Image//scrab.tga");
    meshList[GEO_CRAB_ARM_UPPER] = MeshBuilder::GenerateOBJ("crabArmUpper", "Models//OBJ//scrab_Uarm.obj");
    meshList[GEO_CRAB_ARM_UPPER]->textureArray[0] = LoadTGA("Image//scrab.tga");
    meshList[GEO_CRAB_ARM_MID] = MeshBuilder::GenerateOBJ("crabArmMid", "Models//OBJ//scrab_Marm.obj");
    meshList[GEO_CRAB_ARM_MID]->textureArray[0] = LoadTGA("Image//scrab.tga");
    meshList[GEO_CRAB_ARM_LOWER] = MeshBuilder::GenerateOBJ("crabArmLower", "Models//OBJ//scrab_Larm.obj");
    meshList[GEO_CRAB_ARM_LOWER]->textureArray[0] = LoadTGA("Image//scrab.tga");

    meshList[GEO_FCRABBODY] = MeshBuilder::GenerateOBJ("crabbody", "Models//OBJ//fcrab_body.obj");
    meshList[GEO_FCRABBODY]->textureArray[0] = LoadTGA("Image//fcrab_body.tga");
    meshList[GEO_FCRABLEG] = MeshBuilder::GenerateOBJ("crableg", "Models//OBJ//fcrab_leg.obj");
    meshList[GEO_FCRABLEG]->textureArray[0] = LoadTGA("Image//fcrab_leg.tga");
    meshList[GEO_FCRABCLAW] = MeshBuilder::GenerateOBJ("crabclaw", "Models//OBJ//fcrab_Lclaw.obj");
    meshList[GEO_FCRABCLAW]->textureArray[0] = LoadTGA("Image//fcrab_claw.tga");

    //if (SharedData::GetInstance()->SD_Down)
    //{

    //    walkCam.Init(
    //        Vector3(-814, 253, -1075),
    //        Vector3(10, 0, 0),
    //        Vector3(0, 1, 0),
    //        60
    //        );
    //}
    //else
    //{
    //    walkCam.Init(
    //        Vector3(-765, 175, -136),
    //        Vector3(1, 0, 0.3),
    //        Vector3(0, 1, 0),
    //        60);

    //}

	walkCam.Init(
		Vector3(-146, 371, 663),
		Vector3(1, 0, 0.3),
		Vector3(0, 1, 0),
		60);
    m_travelzoneup = hitbox::generatehitbox(Vector3(-1233, 409, -1263), 600, 500, 600, 0);
    m_travelzonedown = hitbox::generatehitbox(Vector3(-1093, 131, 151), 500, 500, 500, 0);

    //for (int i = 0; i < 20; i++)
    //{
    //    Fcrab *c = FetchFCrab();
    //    c->active = true;
    //    c->objectType = GameObject::SEACREATURE;
    //    c->seaType = SeaCreature::FCRAB;
    //    //c->Cstate = Pufferfish::IDLE;
    //    c->scale.Set(5, 5, 5);
    //    float x = Math::RandFloatMinMax(-300, 0);
    //    float z = Math::RandFloatMinMax(-1000, -800);
    //    float y = 350.f * ReadHeightMap(m_heightMap[2], x / 3000.f, z / 3000.f) + 4;
    //    c->pos.Set(x, y, z);
    //    c->vel.Set(Math::RandFloatMinMax(0, 4), 0, Math::RandFloatMinMax(3, 6));
    //    c->FCstate = Fcrab::IDLE;
    //    c->aabb = hitbox::generatehitbox(c->pos, 8, 8, 8, NULL);
    //    //p->setHealth(200);
    //    g_FCrabCount++;
    //}

	SceneSP3::ReinitCaptured();
    
    giantCrab = new GiantCrab();
    giantCrab->active = true;
    giantCrab->objectType = GameObject::BOSS;
    giantCrab->bossType = Boss::GIANTCRAB;
    giantCrab->setHealth(1000);
    giantCrab->scale.Set(30, 30, 30);
    giantCrab->vel.Set(0, 0, 0);
    giantCrab->pos.Set(-445, giantCrab->pos.y = 350.f * ReadHeightMap(m_heightMap[2], giantCrab->pos.x / 3000.f, giantCrab->pos.z / 3000.f) + 170, 904);
    //giantCrab->direction.Set(1, 0, 0);
    m_goList.push_back(giantCrab);
}

void SceneCreepingRidge::RenderGiantCrab()
{
    modelStack.PushMatrix();
    {
        //Orientation and position
        modelStack.Translate(giantCrab->pos.x, giantCrab->pos.y, giantCrab->pos.z);
        modelStack.Rotate(giantCrab->m_rotate, 0, 1, 0);//rotate entire crab
        modelStack.Scale(70, 70, 70);
        RenderMesh(meshList[GEO_CRAB_BODY], false);


        //Legs

        for (unsigned i = 0; i < 8; ++i)
        {
            static const float rotations[8] =
            {
                -20, 0, 20, 40,
                180 - rotations[3],
                180 - rotations[2],
                180 - rotations[1],
                180 - rotations[0],
            };

            modelStack.PushMatrix();
            {
                modelStack.Translate(0, -0.1, 0);
                modelStack.Rotate(rotations[i], 0, 1, 0);
                modelStack.Rotate(giantCrab->m_leg[i].m_Urotate, 1, 0, 0);
                RenderMesh(meshList[GEO_CRAB_LEG_UPPER], false);
                modelStack.Translate(0.17f, -0.23f, -1.5);
                modelStack.Translate(0, 0.05f, -0.02f);
                modelStack.Rotate(giantCrab->m_leg[i].m_Lrotate, -1, 0, 0);
                modelStack.Translate(0, -0.05f, 0.02f);
                modelStack.Rotate(-6, 0, 1, 0);
                RenderMesh(meshList[GEO_CRAB_LEG_LOWER], false);
            }
            modelStack.PopMatrix();
        }
    }
    modelStack.PopMatrix();


    //modelStack.PushMatrix();//right arm
    //{
    //	modelStack.Translate(0.5f, -0.06f, -0.31f);
    //	modelStack.Rotate(30, 0, 1, 0);
    //	modelStack.Translate(-0.01f, -0.01f, 0);
    //	modelStack.Rotate(0 + 50, 0, 0, 1);
    //	modelStack.Rotate(-20, 1, 0, 0);
    //	modelStack.Translate(0.01f, 0.01f, 0);
    //	modelStack.PushMatrix();
    //	{
    //		modelStack.Translate(3.1f, 0, 0.4f);
    //		modelStack.Rotate(70, 0, 0, -1);
    //		RenderMesh(meshList[GEO_CRAB_ARM_MID], false);

    //		modelStack.PushMatrix();
    //		{
    //			modelStack.Translate(0.83f, 0.1, -0.35);
    //			modelStack.Translate(-0.05f, 0, 0);
    //			modelStack.Rotate(50, 0, 0, -1);
    //			modelStack.Translate(0.05f, 0, 0);
    //			RenderMesh(meshList[GEO_CRAB_ARM_LOWER], false);
    //		}
    //		modelStack.PopMatrix();
    //	}
    //	modelStack.PopMatrix();
    //	RenderMesh(meshList[GEO_CRAB_ARM_UPPER], false);
    //}
    //modelStack.PopMatrix();
    modelStack.PushMatrix();//right arm 	
    modelStack.Translate(giantCrab->m_Rarm.m_Upos.x, giantCrab->m_Rarm.m_Upos.y, giantCrab->m_Rarm.m_Upos.z);
    modelStack.Rotate(giantCrab->m_Rarm.y_upper + giantCrab->m_rotate, 0, 1, 0);//y axis rot, upper arm
    modelStack.Rotate(giantCrab->m_Rarm.x_upper, 0, 0, 1);//x axis rot, upper arm
    modelStack.Scale(70, 70, 70);
    RenderMesh(meshList[GEO_CRAB_ARM_UPPER], false);
    modelStack.PushMatrix();
    modelStack.Translate(2.8, 0, 0.4);
    //modelStack.Rotate(0, 0, 0, -1);
    RenderMesh(meshList[GEO_CRAB_ARM_MID], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(3.6, 0, 0);
    modelStack.Rotate(giantCrab->m_Rarm.x_lower, 0, 0, 1);
    RenderMesh(meshList[GEO_CRAB_ARM_LOWER], false);
    modelStack.PopMatrix();
    modelStack.PopMatrix();


    modelStack.PushMatrix();//left arm 	
    modelStack.Translate(giantCrab->m_Larm.m_Upos.x, giantCrab->m_Larm.m_Upos.y, giantCrab->m_Larm.m_Upos.z);
    modelStack.Rotate(giantCrab->m_Larm.y_upper + giantCrab->m_rotate, 0, 1, 0);//y axis rot, upper arm
    modelStack.Rotate(giantCrab->m_Larm.x_upper, 0, 0, 1);//x axis rot, upper arm
    modelStack.Scale(70, 70, 70);
    RenderMesh(meshList[GEO_CRAB_ARM_UPPER], false);
    modelStack.PushMatrix();
    modelStack.Translate(2.8, 0, 0.4);
    //modelStack.Rotate(0, 0, 0, -1);
    RenderMesh(meshList[GEO_CRAB_ARM_MID], false);
    modelStack.PopMatrix();


    modelStack.PushMatrix();
    modelStack.Translate(3.6, 0, 0);
    modelStack.Rotate(giantCrab->m_Larm.x_lower, 0, 0, 1);
    RenderMesh(meshList[GEO_CRAB_ARM_LOWER], false);
    modelStack.PopMatrix();
    modelStack.PopMatrix();
    //	modelStack.Rotate(-20, 1, 0, 0);
    //	modelStack.Translate(0.01f, 0.01f, 0);
    //	modelStack.PushMatrix();
    //	{
    //		modelStack.Translate(3.1f, 0, 0.4f);
    //		modelStack.Rotate(0, 0, 0, -1);
    //		
    //		RenderMesh(meshList[GEO_CRAB_ARM_MID], false);

    //		modelStack.PushMatrix();
    //		{
    //			modelStack.Translate(0.83f, 0.1, -0.35);
    //			modelStack.Translate(-0.05f, 0, 0);
    //			modelStack.Rotate(giantCrab->m_Larm.x_lower, 0, 0, -1);//x axis rot,lower arm
    //			modelStack.Translate(0.05f, 0, 0);
    //			
    //		}
    //		modelStack.PopMatrix();
    //	}
    //	modelStack.PopMatrix();
    //	RenderMesh(meshList[GEO_CRAB_ARM_UPPER], false);
    //}
    //modelStack.PopMatrix();





}

void SceneCreepingRidge::RenderTerrain()
{
    modelStack.PushMatrix();
    modelStack.Scale(3000.f, 350.0f, 3000.f);
    RenderMesh(meshList[GEO_TERRAIN2], true);
    modelStack.PopMatrix();

}

void SceneCreepingRidge::RenderSkyPlane()
{
    modelStack.PushMatrix();
    modelStack.Translate(0, 2000, 0);
    modelStack.Rotate(rotateSky, 0, 1, 0);
    RenderMesh(meshList[GEO_SKYPLANE], true);
    modelStack.PopMatrix();
}

void SceneCreepingRidge::RenderWorld()
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
	RenderGiantCrab();

}

void SceneCreepingRidge::RenderPassGPass()
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

    //RenderWorld();
}

void SceneCreepingRidge::RenderPassMain()
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
    //RenderWorld();

    SceneSP3::RenderLoop();
    SceneSP3::RenderParticles();
    glUniform1i(m_parameters[U_FOG_ENABLE], 0);

    RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 10.0f, 10.0f);

    RenderMesh(meshList[GEO_AXES], false);

    SceneSP3::RenderMinimap();
    SceneSP3::RenderHUD();


    std::ostringstream ss;
    ss.precision(3);
    ss << "FPS: " << fps;
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 2, 3);

    modelStack.PushMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	//set to line
	modelStack.Translate(giantCrab->grabArea.m_position.x, giantCrab->grabArea.m_position.y, giantCrab->grabArea.m_position.z);
	modelStack.Scale(giantCrab->grabArea.m_width, giantCrab->grabArea.m_height, giantCrab->grabArea.m_length);
    RenderMesh(meshList[GEO_CUBE], false);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	//set back to fill
    modelStack.PopMatrix();


    //modelStack.PushMatrix();
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	//set to line
    //modelStack.Translate(m_travelzoneup.m_position.x, m_travelzoneup.m_position.y, m_travelzoneup.m_position.z);
    //modelStack.Scale(m_travelzoneup.m_width, m_travelzoneup.m_height, m_travelzoneup.m_length);
    //RenderMesh(meshList[GEO_CUBE], false);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	//set back to fill
    //modelStack.PopMatrix();

    for (std::vector<DamageText *>::iterator it = m_textList.begin(); it != m_textList.end(); ++it)
    {
        DamageText *to = (DamageText*)*it;
        if (to->getActive())
            RenderTO(to);
    }
}

void SceneCreepingRidge::Render()
{
    // PRE RENDER PASS
    RenderPassGPass();

    // MAIN RENDER PASS
    RenderPassMain();

}


void SceneCreepingRidge::Update(double dt)
{
    SceneSP3::Update(dt);

    //for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
    //{
    //    GameObject *go = (GameObject*)*it;
    //    if (!go->active)
    //        continue;

    //    if (go->objectType != GameObject::SEACREATURE)
    //        continue;
    //    SeaCreature *so = (SeaCreature*)*it;

    //    if (so->seaType != SeaCreature::FCRAB)
    //        continue;
    //    Fcrab *c = (Fcrab*)*it;

    //    c->pos += c->vel*dt;
    //    float h = 350.f * ReadHeightMap(m_heightMap[2], c->pos.x / 3000.f, c->pos.z / 3000.f) + 4;//get height
    //    //theta=0;
    //    Vector3 displacement = playerpos - c->pos;

    //    if (c->pos.y > 300)// reset crabs who move out of range
    //    {
    //        float x = Math::RandFloatMinMax(-300, 0);
    //        float z = Math::RandFloatMinMax(-1000, -800);
    //        float y = 350.f * ReadHeightMap(m_heightMap[2], x / 3000.f, z / 3000.f) + 4;
    //        c->pos.Set(x, y, z);

    //    }


    //    switch (c->FCstate)
    //    {
    //    case Fcrab::IDLE:
    //    {
    //        if (displacement.LengthSquared() < 50 * 50)
    //        {
    //            c->pos.y += 1;
    //            theta = Math::RadianToDegree(atan2(displacement.y, Vector3(displacement.x, 0, displacement.z).Length()));
    //            c->vel = displacement.Normalized() * 30;
    //            c->FCstate = Fcrab::ATTACKING;
    //        }

    //        if (c->pos.y < h)
    //            c->pos.y += dt * 10;
    //        else if (c->pos.y > h)
    //            c->pos.y -= dt * 10;
    //        break;
    //    }

    //    case Fcrab::ATTACKING:

    //        c->vel.y -= 9.8*dt;



    //        if (collision(c->aabb, player_box))
    //        {
    //            fishVel = -c->vel;
    //        }

    //        if (c->pos.y <= h)
    //        {
    //            c->vel.Set(Math::RandFloatMinMax(0, 4), 0, Math::RandFloatMinMax(3, 6));

    //            c->pos.y = h;
    //            //std::cout << "idling" << std::endl;
    //            c->FCstate = Fcrab::IDLE;
    //            break;

    //        }

    //        //c->vel *= Math::RadianToDegree(sin(theta))/(9.8*dt);

    //        //if (displacement.LengthSquared() < 20 * 20)
    //        //c->FCstate = Fcrab::ATTACKING;
    //        break;
    //    }


    //    (c)->UpdateFcrab(dt);//run update for fcrabs

    //}

	//giantCrab->pos.y = 350.f * ReadHeightMap(m_heightMap[2], giantCrab->pos.x / 3000.f, giantCrab->pos.z / 3000.f) + 170;
	//giantCrab->updateGC(dt);

    giantCrab->UpdateGC(dt, m_heightMap[2]);
    if (collision(giantCrab->grabArea, player_box))
    {
        giantCrab->SetState(GiantCrab::GRAB);
    }



    if (giantCrab->GetState() == GiantCrab::VORTEX)
    {

        Vector3 target = playerpos - giantCrab->grabArea.m_position;
        for (auto it : particleList)
        {
            ParticleObject* p = (ParticleObject*)it;
            if (!p->active)
                continue;
            if (p->type != PARTICLEOBJECT_TYPE::P_PARTICLE)
                continue;

            p->vel = -target.Normalized() * 200;

        }

        fishVel += -15 * giantCrab->vel;
        fishVel.y = -target.y;

    }
}

void SceneCreepingRidge::Exit()
{
    SceneSP3::Exit();
}