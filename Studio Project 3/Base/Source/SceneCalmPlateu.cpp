#include "SceneCalmPlateu.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

using std::cout;
using std::endl;

SceneCalmPlateu::SceneCalmPlateu()
{

}

SceneCalmPlateu::~SceneCalmPlateu()
{

}

void SceneCalmPlateu::Init()
{
    SceneSP3::Init();
    if (SharedData::GetInstance()->SD_Down)
    {
        walkCam.Init(
            Vector3(-1171, 277, -812),
            Vector3(0, 0, 10),
            Vector3(0, 1, 0),
            60
            );
    }
    else
    {
        walkCam.Init(
            Vector3(1274, 322, 42),
            Vector3(-1, 0, 0),
            Vector3(0, 1, 0),
            60
            );
    }

    //walkCam.Init(
    //    Vector3(0, 500, 0),
    //    Vector3(0, 0, 10),
    //    Vector3(0, 1, 0),
    //    60
    //    );


    for (int i = 0; i < 10; i++)
    {
        Pufferfish *p = FetchPuffer();
        p->active = true;
        p->objectType = GameObject::SEACREATURE;
        p->seaType = SeaCreature::PUFFER;
        p->pstate = Pufferfish::IDLE;
        p->scale.Set(5, 5, 5);
        p->pos.Set(0, 500, 0);
        p->vel.Set(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10));
        p->collision = hitbox2::generatehitbox(p->pos, 8, 8, 8);
        p->setHealth(200);
        p->setMoveCoolDown(0.0);
        g_PufferfishCount++;
    }

    m_travelzonedown = hitbox::generatehitbox(Vector3(1386, 295, 5.8), 200, 600, 600, 0);
    m_travelzoneup = hitbox::generatehitbox(Vector3(-1258, 389, -1221), 500, 700, 500, 0);

    InitGiantSquid();

    isPlayerHit = false;
	SceneSP3::ReinitCaptured();
	
}

void SceneCalmPlateu::InitGiantSquid()
{
    giantSquid = new GiantSquid();
    giantSquid->active = true;
    giantSquid->objectType = GameObject::BOSS;
    giantSquid->bossType = Boss::GIANTSQUID;
    giantSquid->state = GiantSquid::IDLE;
    giantSquid->setHealth(1000);
    giantSquid->scale.Set(30, 30, 30);
    giantSquid->pos.Set(-100, 500, 0);
    giantSquid->vel.Set(0, 0, 0);
    giantSquid->setSquidView(Vector3(0, 0, 0));
    giantSquid->setIsDead(false);
    giantSquid->m_ChangeState = false;
    giantSquid->m_isSquidUp = false;
    giantSquid->m_rotateComplete = false;
    giantSquid->m_isSpinning = false;
    giantSquid->m_isInkTentacleRotated = false;
    giantSquid->m_isShootInk = false;
    giantSquid->m_isTentacleUp = false;
    giantSquid->m_rotateSquid = 0.f;
    giantSquid->m_rotateSpinTentacle = 0.f;
    giantSquid->m_rotateInkTentacle = 0.f;
    giantSquid->m_translateSquid = 0.f;
    giantSquid->m_InkFiredCount = 0;
    giantSquid->m_spinSpeed = 1.f;
    giantSquid->m_moveSpeed = 1.f;
    giantSquid->m_rotateTentacle = 0.f;
    giantSquid->collision = hitbox::generatehitbox(giantSquid->pos + Vector3(0, 80, 0), 55, 150, 55, 0);

    for (int i = 0; i < 6; ++i)
    {
        giantSquid->tentacle[i] = new GiantSquidTentacle();
        giantSquid->tentacle[i]->setTentaclePos(giantSquid->pos);
        giantSquid->tentacle[i]->setScale(Vector3(10, 30, 10));
        giantSquid->tentacle[i]->setHealth(50);
        giantSquid->tentacle[i]->m_active = true;
        giantSquid->tentacle[i]->collision = hitbox::generatehitbox(giantSquid->pos, giantSquid->tentacle[i]->getScale().x, giantSquid->tentacle[i]->getScale().y, giantSquid->tentacle[i]->getScale().z, 0);
    }

    m_goList.push_back(giantSquid);
}

Pufferfish* SceneCalmPlateu::FetchPuffer()
{
    for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
    {
        GameObject *go = (GameObject *)*it;
        if (!go->active)
        {
            Pufferfish *pf = (Pufferfish *)*it;
            pf->active = true;
            return pf;
        }

    }
    for (unsigned i = 0; i < 10; ++i)
    {
        Pufferfish *pf = new Pufferfish();
        pf->objectType = GameObject::SEACREATURE;
        pf->seaType = SeaCreature::PUFFER;
        m_goList.push_back(pf);
    }
    Pufferfish *pf = (Pufferfish *)m_goList.back();
    pf->active = true;
    return pf;
}

void SceneCalmPlateu::RenderGiantSquid()
{
    if (giantSquid)
    {
        modelStack.PushMatrix();
        modelStack.Translate(giantSquid->pos.x, giantSquid->pos.y, giantSquid->pos.z);
        modelStack.Rotate(giantSquid->m_rotateSquid + giantSquid->getSquidLookAt(), 0, 1, 0);
        modelStack.Scale(giantSquid->scale.x, giantSquid->scale.y, giantSquid->scale.z);
        RenderMesh(meshList[GEO_SQUIDBODY], false);

        // Part 1
        if (giantSquid->tentacle[0]->m_active)
        {
            modelStack.PushMatrix();
            modelStack.Translate(-.25f, .1f, -.15f);
            modelStack.Rotate(135.f, 0, 1, 0);
            modelStack.Rotate(giantSquid->m_rotateTentacle, 0, 0, 1);
            modelStack.Rotate(giantSquid->m_rotateInkTentacle, 0, 0, 1);
            RenderMesh(meshList[GEO_SQUIDTENTACLENODE], false);

            modelStack.PushMatrix();
            modelStack.Translate(0, -1.f, 0);
            modelStack.Rotate(giantSquid->m_rotateTentacle, 0, 0, 1);
            RenderMesh(meshList[GEO_SQUIDTENTACLENODE], false);

            modelStack.PushMatrix();
            modelStack.Translate(0, -1.f, 0);
            modelStack.Rotate(giantSquid->m_rotateTentacle, 0, 0, 1);
            RenderMesh(meshList[GEO_SQUIDTENTACLENODE], false);

            modelStack.PushMatrix();
            modelStack.Translate(0, -1.f, 0);
            modelStack.Rotate(giantSquid->m_rotateTentacle, 0, 0, 1);
            RenderMesh(meshList[GEO_SQUIDTENTACLEEND], false);

            modelStack.PopMatrix();
            modelStack.PopMatrix();
            modelStack.PopMatrix();
            modelStack.PopMatrix();
        }


        // Part 2
        if (giantSquid->tentacle[5]->m_active)
        {
            modelStack.PushMatrix();
            modelStack.Translate(-.25f, .1f, .15f);
            modelStack.Rotate(225.f, 0, 1, 0);
            modelStack.Rotate(giantSquid->m_rotateTentacle, 0, 0, 1);
            modelStack.Rotate(giantSquid->m_rotateInkTentacle, 0, 0, 1);
            RenderMesh(meshList[GEO_SQUIDTENTACLENODE], false);

            modelStack.PushMatrix();
            modelStack.Translate(0, -1.f, 0);
            modelStack.Rotate(giantSquid->m_rotateTentacle, 0, 0, 1);
            RenderMesh(meshList[GEO_SQUIDTENTACLENODE], false);

            modelStack.PushMatrix();
            modelStack.Translate(0, -1.f, 0);
            modelStack.Rotate(giantSquid->m_rotateTentacle, 0, 0, 1);
            RenderMesh(meshList[GEO_SQUIDTENTACLENODE], false);

            modelStack.PushMatrix();
            modelStack.Translate(0, -1.f, 0);
            modelStack.Rotate(giantSquid->m_rotateTentacle, 0, 0, 1);
            RenderMesh(meshList[GEO_SQUIDTENTACLEEND], false);

            modelStack.PopMatrix();
            modelStack.PopMatrix();
            modelStack.PopMatrix();
            modelStack.PopMatrix();
        }

        // Part 3
        if (giantSquid->tentacle[1]->m_active)
        {
            modelStack.PushMatrix();
            modelStack.Translate(.15f, .1f, -.15f);
            modelStack.Rotate(45.f, 0, 1, 0);
            modelStack.Rotate(giantSquid->m_rotateTentacle, 0, 0, 1);
            modelStack.Rotate(giantSquid->m_rotateInkTentacle, 0, 0, 1);
            RenderMesh(meshList[GEO_SQUIDTENTACLENODE], false);

            modelStack.PushMatrix();
            modelStack.Translate(0, -1.f, 0);
            modelStack.Rotate(giantSquid->m_rotateTentacle, 0, 0, 1);
            RenderMesh(meshList[GEO_SQUIDTENTACLENODE], false);

            modelStack.PushMatrix();
            modelStack.Translate(0, -1.f, 0);
            modelStack.Rotate(giantSquid->m_rotateTentacle, 0, 0, 1);
            RenderMesh(meshList[GEO_SQUIDTENTACLENODE], false);

            modelStack.PushMatrix();
            modelStack.Translate(0, -1.f, 0);
            modelStack.Rotate(giantSquid->m_rotateTentacle, 0, 0, 1);
            RenderMesh(meshList[GEO_SQUIDTENTACLEEND], false);

            modelStack.PopMatrix();
            modelStack.PopMatrix();
            modelStack.PopMatrix();
            modelStack.PopMatrix();
        }

        // Part 4
        if (giantSquid->tentacle[4]->m_active)
        {
            modelStack.PushMatrix();
            modelStack.Translate(.15f, .1f, .15f);
            modelStack.Rotate(-45.f, 0, 1, 0);
            modelStack.Rotate(giantSquid->m_rotateTentacle, 0, 0, 1);
            modelStack.Rotate(giantSquid->m_rotateInkTentacle, 0, 0, 1);
            RenderMesh(meshList[GEO_SQUIDTENTACLENODE], false);

            modelStack.PushMatrix();
            modelStack.Translate(0, -1.f, 0);
            modelStack.Rotate(giantSquid->m_rotateTentacle, 0, 0, 1);
            RenderMesh(meshList[GEO_SQUIDTENTACLENODE], false);

            modelStack.PushMatrix();
            modelStack.Translate(0, -1.f, 0);
            modelStack.Rotate(giantSquid->m_rotateTentacle, 0, 0, 1);
            RenderMesh(meshList[GEO_SQUIDTENTACLENODE], false);

            modelStack.PushMatrix();
            modelStack.Translate(0, -1.f, 0);
            modelStack.Rotate(giantSquid->m_rotateTentacle, 0, 0, 1);
            RenderMesh(meshList[GEO_SQUIDTENTACLEEND], false);

            modelStack.PopMatrix();
            modelStack.PopMatrix();
            modelStack.PopMatrix();
            modelStack.PopMatrix();
        }


        // Part 5
        if (giantSquid->tentacle[2]->m_active)
        {
            modelStack.PushMatrix();
            modelStack.Translate(-.05f, .1f, -.35f);
            modelStack.Rotate(90.f, 0, 1, 0);
            modelStack.Rotate(giantSquid->m_rotateTentacle, 0, 0, 1);
            modelStack.Rotate(giantSquid->m_rotateInkTentacle, 0, 0, 1);
            RenderMesh(meshList[GEO_SQUIDTENTACLENODE], false);

            modelStack.PushMatrix();
            modelStack.Translate(0, -1.f, 0);
            modelStack.Rotate(giantSquid->m_rotateTentacle, 0, 0, 1);
            RenderMesh(meshList[GEO_SQUIDTENTACLENODE], false);

            modelStack.PushMatrix();
            modelStack.Translate(0, -1.f, 0);
            modelStack.Rotate(giantSquid->m_rotateTentacle, 0, 0, 1);
            RenderMesh(meshList[GEO_SQUIDTENTACLENODE], false);

            modelStack.PushMatrix();
            modelStack.Translate(0, -1.f, 0);
            modelStack.Rotate(giantSquid->m_rotateTentacle, 0, 0, 1);
            RenderMesh(meshList[GEO_SQUIDTENTACLEEND], false);

            modelStack.PopMatrix();
            modelStack.PopMatrix();
            modelStack.PopMatrix();
            modelStack.PopMatrix();
        }

        // Part 6
        if (giantSquid->tentacle[3]->m_active)
        {
            modelStack.PushMatrix();
            modelStack.Translate(-.05f, .1f, .35f);
            modelStack.Rotate(-90.f, 0, 1, 0);
            modelStack.Rotate(giantSquid->m_rotateTentacle, 0, 0, 1);
            modelStack.Rotate(giantSquid->m_rotateInkTentacle, 0, 0, 1);
            RenderMesh(meshList[GEO_SQUIDTENTACLENODE], false);

            modelStack.PushMatrix();
            modelStack.Translate(0, -1.f, 0);
            modelStack.Rotate(giantSquid->m_rotateTentacle, 0, 0, 1);
            RenderMesh(meshList[GEO_SQUIDTENTACLENODE], false);

            modelStack.PushMatrix();
            modelStack.Translate(0, -1.f, 0);
            modelStack.Rotate(giantSquid->m_rotateTentacle, 0, 0, 1);
            RenderMesh(meshList[GEO_SQUIDTENTACLENODE], false);

            modelStack.PushMatrix();
            modelStack.Translate(0, -1.f, 0);
            modelStack.Rotate(giantSquid->m_rotateTentacle, 0, 0, 1);
            RenderMesh(meshList[GEO_SQUIDTENTACLEEND], false);

            modelStack.PopMatrix();
            modelStack.PopMatrix();
            modelStack.PopMatrix();
            modelStack.PopMatrix();
        }

        modelStack.PopMatrix();
    }
}

void SceneCalmPlateu::RenderTerrain()
{
    modelStack.PushMatrix();
    modelStack.Scale(3000.f, 350.0f, 3000.f);
    RenderMesh(meshList[GEO_TERRAIN1], true);
    modelStack.PopMatrix();

}

void SceneCalmPlateu::RenderSkyPlane()
{
    modelStack.PushMatrix();
    modelStack.Translate(0, 2000, 0);
    modelStack.Rotate(rotateSky, 0, 1, 0);
    RenderMesh(meshList[GEO_SKYPLANE], true);
    modelStack.PopMatrix();
}

void SceneCalmPlateu::RenderWorld()
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
}

void SceneCalmPlateu::RenderPassGPass()
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

void SceneCalmPlateu::RenderPassMain()
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

    for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
    {
        GameObject *go = (GameObject*)*it;

        if (!go->active)
            continue;
        if (go->objectType == GameObject::SEACREATURE)
        {
            SeaCreature * fo = (SeaCreature*)*it;

            switch (fo->seaType)
            {
            case SeaCreature::MINNOW:
            {
                Minnow * m = (Minnow*)*it;
                RenderFO(m);
                break;
            }
            case SeaCreature::PUFFER:
            {
                Pufferfish * p = (Pufferfish*)*it;
                RenderPuffer(p);
                break;
            }
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
		else if (go->objectType == GameObject::CAPTURED)
		{
			SeaCreature* c = (SeaCreature*)go;
			RenderSquad(c);
		}
    }

    RenderGiantSquid();

    modelStack.PushMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	//set to line
    modelStack.Translate(giantSquid->collision.m_position.x, giantSquid->collision.m_position.y, giantSquid->collision.m_position.z);
    modelStack.Scale(giantSquid->collision.m_width, giantSquid->collision.m_height, giantSquid->collision.m_length);
    RenderMesh(meshList[GEO_CUBE], false);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	//set back to fill
    modelStack.PopMatrix();

    for (int i = 0; i < 6; ++i)
    {
        modelStack.PushMatrix();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	//set to line
        modelStack.Translate(giantSquid->tentacle[i]->collision.m_position.x, giantSquid->tentacle[i]->collision.m_position.y, giantSquid->tentacle[i]->collision.m_position.z);
        modelStack.Scale(giantSquid->tentacle[i]->getScale().x, giantSquid->tentacle[i]->getScale().y, giantSquid->tentacle[i]->getScale().z);
        RenderMesh(meshList[GEO_CUBE], false);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	//set back to fill
        modelStack.PopMatrix();
    }

    glUniform1i(m_parameters[U_FOG_ENABLE], 0);
    // Render the crosshair
    RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 10.0f, 10.0f);

    RenderMesh(meshList[GEO_AXES], false);

    SceneSP3::RenderParticles();
    SceneSP3::RenderMinimap();
    SceneSP3::RenderHUD();

    // Travel Zone
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

    std::ostringstream ss;
    ss.precision(3);
    ss << "FPS: " << fps;
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 2, 3);
}

void SceneCalmPlateu::Render()
{
    // PRE RENDER PASS
    RenderPassGPass();

    // MAIN RENDER PASS
    RenderPassMain();

}

void SceneCalmPlateu::RenderPuffer(Pufferfish *fo)
{
    float rotate = 0;
    rotate = Math::RadianToDegree(atan2(fo->vel.x, fo->vel.z));
    modelStack.PushMatrix();
    modelStack.Translate(fo->pos.x, fo->pos.y, fo->pos.z);
    modelStack.Rotate(rotate, 0, 1, 0);
    modelStack.Scale(fo->scale.x, fo->scale.y, fo->scale.z);
    RenderMesh(meshList[GEO_PUFFER], false);
    modelStack.PopMatrix();

}

void SceneCalmPlateu::Update(double dt)
{
    SceneSP3::Update(dt);
    //UpdatePuffer(dt);
    //UpdateGiantSquid(dt);
    UpdateParticles(dt);

    hitbox::updatehitbox(giantSquid->collision, giantSquid->collision.m_position);

    //if (collision(giantSquid->collision, playerpos))
    //{
    //    fishVel *= -1.f;
    //    walkCam.Move(fishVel * (float)dt);
    //    playerpos = walkCam.GetPos() + Vector3(0, 80, 0);
    //    hitbox2::updatehitbox(player_box, playerpos);
    //}

    for (int i = 0; i < 6; i++)
    {
        hitbox::updatehitbox(giantSquid->tentacle[i]->collision, giantSquid->tentacle[i]->collision.m_position);

        if (giantSquid->tentacle[i]->getHealth() < 0)
            giantSquid->tentacle[i]->m_active = false;

        if (!giantSquid->tentacle[i]->m_active)
            giantSquid->tentacle[i]->collision.m_position = Vector3(0, 0, 0);
    }


    PufferfishSpawner.CheckCount(g_PufferfishCount, g_MaxPufferfish);

    if (PufferfishSpawner.getIsSpawn())
    {
        Vector3 tv(Math::RandFloatMinMax(-1000.f, 1000.f), Math::RandFloatMinMax(0, 1000.f), Math::RandFloatMinMax(-1000.f, 1000.f));
        if (!terraincollision(tv, m_heightMap[SharedData::GetInstance()->SD_CurrentArea]))
        {
            pf = new Pufferfish();
            pf->active = true;
            pf->objectType = GameObject::SEACREATURE;
            pf->seaType = SeaCreature::PUFFER;
            pf->pstate = Pufferfish::IDLE;
            pf->scale.Set(5, 5, 5);
            pf->pos.Set(tv.x, tv.y, tv.z);
            pf->vel.Set(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10));
            pf->collision = hitbox2::generatehitbox(pf->pos, 8, 8, 8);
            pf->setHealth(200);
            pf->setMoveCoolDown(0.0);
            g_PufferfishCount++;
            m_goList.push_back(pf);
        }
    }
}

void SceneCalmPlateu::UpdateGiantSquid(double dt)
{
    if (giantSquid->m_ChangeState)
    {
        int random = 0;
        random = rand() % 10 + 1;

        bool canSpin = false;

        for (int i = 0; i < 6; ++i)
        {
            if (giantSquid->tentacle[i]->m_active)
            {
                canSpin = true;
                break;
            }
        }

        if (giantSquid->getHealth() < 0)
        {
            giantSquid->state = GiantSquid::DEAD;
        }
        else if ((giantSquid->pos - playerpos).LengthSquared() < g_distFromGiantSquid * g_distFromGiantSquid && random > 6 && canSpin)
        {
            giantSquid->state = GiantSquid::SPINATTACK;
            isPlayerHit = false;
        }
        else if ((giantSquid->pos - playerpos).LengthSquared() < g_distFromGiantSquid * g_distFromGiantSquid && random < 7)
        {
            giantSquid->state = GiantSquid::INKATTACK;
        }
        else
        {
            giantSquid->state = GiantSquid::IDLE;
        }

        giantSquid->m_ChangeState = false;
    }

    switch (giantSquid->state)
    {
    case GiantSquid::IDLE:
        giantSquid->AnimateIdle();
        break;
    case GiantSquid::SPINATTACK:
        giantSquid->AnimateSpinAttack();
        giantSquid->setSquidView(giantSquid->LookAtPlayer(playerpos));
        giantSquid->ChasePlayer(playerpos);
        if ((giantSquid->pos - playerpos).LengthSquared() < 10000 && giantSquid->m_isSpinning && !isPlayerHit)
        {
            fishVel *= -10;
            walkCam.Move(fishVel * (float)dt);
            playerpos = walkCam.GetPos() + Vector3(0, 80, 0);
            skipper->setHealth(skipper->getHealth() - 50);
            isPlayerHit = true;
        }
        break;
    case GiantSquid::INKATTACK:
        giantSquid->AnimateInkAttack();
        giantSquid->setSquidView(giantSquid->LookAtPlayer(playerpos));
        if (giantSquid->m_isShootInk)
        {
            for (int x = -5; x <= 5; x++)
            {
                Projectile *po = FetchPO();
                po->objectType = GameObject::PROJECTILE;
                po->projectileType = Projectile::INK;
                po->active = true;
                po->scale.Set(1, 1, 1);
                po->setLifetime(1.0);
                po->pos.Set(Math::RandFloatMinMax(giantSquid->pos.x - g_inkSpread, giantSquid->pos.x + g_inkSpread),
                    Math::RandFloatMinMax(giantSquid->pos.y + x - g_inkSpread, giantSquid->pos.y + x + g_inkSpread),
                    Math::RandFloatMinMax(giantSquid->pos.z - g_inkSpread, giantSquid->pos.z + g_inkSpread));
                Vector3 view = (playerpos - giantSquid->pos).Normalized();
                po->vel.Set(view.x, view.y, view.z);
                po->setLifetime(1.0);
            }
            for (int x = -5; x <= 5; x++)
            {
                Projectile *po = FetchPO();
                po->objectType = GameObject::PROJECTILE;
                po->projectileType = Projectile::INK;
                po->active = true;
                po->scale.Set(1, 1, 1);
                po->setLifetime(1.0);
                po->pos.Set(Math::RandFloatMinMax(giantSquid->pos.x + x - g_inkSpread, giantSquid->pos.x + x + g_inkSpread),
                    Math::RandFloatMinMax(giantSquid->pos.y - g_inkSpread, giantSquid->pos.y + g_inkSpread),
                    Math::RandFloatMinMax(giantSquid->pos.z - g_inkSpread, giantSquid->pos.z + g_inkSpread));
                Vector3 view = (playerpos - giantSquid->pos).Normalized();
                po->vel.Set(view.x, view.y, view.z);
                po->setLifetime(1.0);
            }
        }
        break;
    case GiantSquid::DEAD:
        giantSquid->AnimateDead();
        if (giantSquid->getIsDead())
            giantSquid->active = false;
        break;
    }
}

void SceneCalmPlateu::UpdatePuffer(double dt)
{
    //for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
    //{
    //	GameObject *go = (GameObject *)*it;
    //	if (!go->active)
    //		continue;

    //	if (go->objectType != GameObject::SEACREATURE)
    //		continue;

    //	Pufferfish *p = (Pufferfish *)*it;

    //	if (p->seaType != SeaCreature::PUFFER)
    //		continue;


    //	hitbox2::updatehitbox(p->collision, p->pos);
    //	switch (p->pstate)
    //	{
    //	case Pufferfish::IDLE:
    //	{
    //		p->pos += p->vel.Normalize() * dt * 10;
    //		
    //		//check bullet collision
    //		for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
    //		{
    //			GameObject *b = (GameObject *)*it;
    //			if (b->objectType == GameObject::PROJECTILE && b->active)
    //			{
    //				
    //				Vector3 displacement = p->pos - b->pos;
    //				if (displacement.LengthSquared() <  p->scale.z + b->scale.z + 50)
    //				{
    //					p->scale = Vector3(10, 10, 10);
    //					//p->collision.m_height
    //					p->pstate = Pufferfish::ENRAGED;
    //					std::cout << " hit" << std::endl;
    //					break;
    //				}
    //			}
    //		}
    //		p->pos += p->vel.Normalize() * dt * 10;

    //	}break;
    //	case Pufferfish::ENRAGED:
    //	{
    //		Vector3 displacement = playerpos - p->pos;
    //		if (displacement.LengthSquared() > 400*400)
    //		{
    //			p->vel *= -1.f;
    //			p->scale = Vector3(5, 5, 5);
    //			p->pstate = Pufferfish::IDLE;
    //		}
    //		p->vel = displacement.Normalize();
    //		if (collision(p->collision, player_box))
    //		{
    //			//p->vel *= -1.f;
    //			if (fishVel.Length() < 3)
    //				fishVel = Vector3(5, 5, 5);

    //			fishVel *= -5.f;
    //			walkCam.Move(fishVel * (float)dt);
    //			playerpos = walkCam.GetPos() + Vector3(0, 80, 0);
    //		}

    //		p->pos += p->vel * dt * 20;
    //		
    //	}break;
    //	}
    //	
    //	if (terraincollision(p->collision, m_heightMap[SharedData::GetInstance()->SD_CurrentArea]))//check collision;
    //	{
    //		p->vel *= -5.f;
    //		p->pos.y += 4;
    //	}
    //	if (p->pos.y > 650)//heigt limit
    //	{
    //		p->pos.y -= 1;
    //		p->vel.y = -p->vel.y;
    //	}
    //
    //	// Cap velocity
    //	if (p->vel.x > 20)
    //		p->vel.x = 20;
    //	if (p->vel.y > 20)
    //		p->vel.y = 20;
    //	if (p->vel.z > 20)
    //		p->vel.z = 20;
    //	if (p->vel.x < -20)
    //		p->vel.x = -20;
    //	if (p->vel.y < -20)
    //		p->vel.y = -20;
    //	if (p->vel.z < -20)
    //		p->vel.z = -20;

    //}
}

void SceneCalmPlateu::Exit()
{
    for (int i = 0; i < 6; ++i)
        delete giantSquid->tentacle[i];
    SceneSP3::Exit();
}