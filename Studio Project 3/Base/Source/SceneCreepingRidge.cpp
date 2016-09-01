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
	glClearColor(0.0f, 0.65f, 0.85f,0.f);
	Color fogColor(0.0f, 0.65f, 0.85f);
	glUniform3fv(m_parameters[U_FOG_COLOR], 1, &fogColor.r);
	glUniform1f(m_parameters[U_FOG_END], 1300);
	glUniform1f(m_parameters[U_FOG_THICKNESS], 1.5f);

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

    if (SharedData::GetInstance()->SD_Down)
    {

        walkCam.Init(
            Vector3(-814, 253, -1075),
            Vector3(10, 0, 0),
            Vector3(0, 1, 0),
            60
            );
    }
    else
    {
        walkCam.Init(
            Vector3(-765, 175, -136),
            Vector3(1, 0, 0.3),
            Vector3(0, 1, 0),
            60);

    }


    m_travelzoneup = hitbox::generatehitbox(Vector3(-1233, 409, -1263), 600, 500, 600, 0);
    m_travelzonedown = hitbox::generatehitbox(Vector3(-1093, 131, 151), 500, 500, 500, 0);
    
    giantCrab = new GiantCrab();
    giantCrab->active = true;
    giantCrab->objectType = GameObject::BOSS;
    giantCrab->bossType = Boss::GIANTCRAB;
    giantCrab->setHealth(1000);
    giantCrab->scale.Set(30, 30, 30);
    giantCrab->vel.Set(0, 0, 0);
    giantCrab->pos.Set(-445, giantCrab->pos.y = 350.f * ReadHeightMap(m_heightMap[2], giantCrab->pos.x / 3000.f, giantCrab->pos.z / 3000.f) + 170, 904);
    seaList.push_back(giantCrab);
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

    modelStack.PushMatrix();//right arm 	
    modelStack.Translate(giantCrab->m_Rarm.m_Upos.x, giantCrab->m_Rarm.m_Upos.y, giantCrab->m_Rarm.m_Upos.z);
    modelStack.Rotate(giantCrab->m_Rarm.y_upper + giantCrab->m_rotate, 0, 1, 0);//y axis rot, upper arm
    modelStack.Rotate(giantCrab->m_Rarm.x_upper, 0, 0, 1);//x axis rot, upper arm
    modelStack.Scale(70, 70, 70);
    RenderMesh(meshList[GEO_CRAB_ARM_UPPER], false);
    modelStack.PushMatrix();
    modelStack.Translate(2.8, 0, 0.4);
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
    RenderMesh(meshList[GEO_CRAB_ARM_MID], false);
    modelStack.PopMatrix();


    modelStack.PushMatrix();
    modelStack.Translate(3.6, 0, 0);
    modelStack.Rotate(giantCrab->m_Larm.x_lower, 0, 0, 1);
    RenderMesh(meshList[GEO_CRAB_ARM_LOWER], false);
    modelStack.PopMatrix();
    modelStack.PopMatrix();
}

void SceneCreepingRidge::RenderTerrain()
{
    modelStack.PushMatrix();
    modelStack.Scale(3000.f, 350.0f, 3000.f);
    RenderMesh(meshList[GEO_TERRAIN2], true);
    modelStack.PopMatrix();

}

void SceneCreepingRidge::RenderWorld()
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
    modelStack.Translate(0, -0.02, -1.2);
    modelStack.Rotate(fish_tailrot, 0, 1, 0);

    RenderMesh(meshList[GEO_FISHTAIL], true);
    modelStack.PopMatrix();
    modelStack.PopMatrix();

	if (!SharedData::GetInstance()->SD_BossDead2)
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

    projectionStack.LoadMatrix(currentCam->GetProjection());
    viewStack.LoadMatrix(currentCam->GetView());
    // Model matrix : an identity matrix (model will be at the origin)
    modelStack.LoadIdentity();
    RenderWorld();

    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

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

    if (!SharedData::GetInstance()->SD_BossDead2 && !giantCrab->isstunned && !isGamePaused)
    {
        giantCrab->UpdateGC(dt, m_heightMap[2]);
        if (collision(giantCrab->grabArea, player_box) && giantCrab->GetState() != GiantCrab::STRAFE)
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

            fishVel += -5 * target;
            fishVel.y = -target.y;

        }

    }
    if (giantCrab->GetState() == GiantCrab::GRAB)
    {
        fishVel.SetZero();
		if (skipper->getTimerReceieveDamage() > 1.0)
		{
			skipper->setTimerReceieveDamage(0.0);
			skipper->setHealth(skipper->getHealth() - 50);
			DamageText* text = FetchTO();
			text->setActive(true);
			text->setLastHitPos(playerpos + walkCam.GetDir().Normalized() * 5 + Vector3(0, 10, 0));
			text->setLastDamage(20);
			text->setScaleText(Vector3(0, 0, 0));
			text->setIsEnemy(false);
		}
    }
    if (collision(giantCrab->m_hitbox, player_box))
    {
        fishVel = -giantCrab->vel * 10;
        if (skipper->getTimerReceieveDamage() > 1.0)
        {
            skipper->setTimerReceieveDamage(0.0);
            skipper->setHealth(skipper->getHealth() - 40);
            DamageText* text = FetchTO();
            text->setActive(true);
            text->setLastHitPos(playerpos + walkCam.GetDir().Normalized() * 5 + Vector3(0, 10, 0));
            text->setLastDamage(20);
            text->setScaleText(Vector3(0, 0, 0));
            text->setIsEnemy(false);
        }
    }
}


void SceneCreepingRidge::Exit()
{
    SceneSP3::Exit();
}