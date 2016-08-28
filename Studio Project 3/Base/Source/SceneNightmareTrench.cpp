#include "SceneNightmareTrench.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneNightmareTrench::SceneNightmareTrench()
{

}

SceneNightmareTrench::~SceneNightmareTrench()
{

}

void SceneNightmareTrench::Init()
{
	SceneSP3::Init();
	//currentCam = &walkCam;
	/*walkCam.Init(
		Vector3(980, 279, -1040),
		Vector3(-1, 0, 1),
		Vector3(0, 1, 0),
		60
		);*/

	walkCam.Init(
		Vector3(0, 500, 0),
		Vector3(-1, 0, 1),
		Vector3(0, 1, 0),
		60);

	m_travelzoneup = hitbox::generatehitbox(Vector3(1173,372,-1230),300,500,300,0);
	for (unsigned i = 0; i < 20; i++)
	{
		Chimera*c = FetchChimera();
		c->active = true;
		c->objectType = GameObject::SEACREATURE;
		c->seaType = SeaCreature::CHIMERA;
		//p->pstate = Pufferfish::IDLE;
		c->scale.Set(20, 20, 20);
		c->pos.Set(Math::RandFloatMinMax(-1000, 1000), Math::RandFloatMinMax(100, 500), Math::RandFloatMinMax(-1000, 1000));
		c->vel.Set(0, Math::RandFloatMinMax(-20, 20), 0);
		c->aabb = hitbox::generatehitbox(c->pos, 10, 10, 10,NULL);
		c->setHealth(200);
	}

	isopod = new Isopod;
}

void SceneNightmareTrench::RenderTerrain()
{
	modelStack.PushMatrix();
	modelStack.Scale(3000.f, 350.0f, 3000.f);
	RenderMesh(meshList[GEO_TERRAIN4], true);
	modelStack.PopMatrix();

}

void SceneNightmareTrench::RenderSkyPlane()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 2000, 0);
	modelStack.Rotate(rotateSky, 0, 1, 0);
	RenderMesh(meshList[GEO_SKYPLANE], true);
	modelStack.PopMatrix();
}

void SceneNightmareTrench::RenderBoss()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 500, 0);
	modelStack.Scale(80, 80, 80);
	//modelStack.Rotate(rotateSky, 0, 1, 0);
	RenderMesh(meshList[GEO_ISOPOD_BODY], false);

	for (unsigned i = 0; i < 6; i++)
	{
		float rr = 60 * sin(isopod->m_Rleg[i].roll);
		modelStack.PushMatrix();//right side
		modelStack.Translate(0.6-i*0.3, 0.08, -0.4+i*0.05);
		modelStack.Rotate(90+rr, 0, 1, 0);
		RenderMesh(meshList[GEO_ISOPOD_LEG], false);
		modelStack.PopMatrix();

		float rl = 60 * sin(isopod->m_Lleg[i].roll);
		modelStack.PushMatrix();//left side
		modelStack.Translate(0.6 - i*0.3, 0.08, 0.4-i*0.05);
		modelStack.Rotate(-90-rl, 0, 1, 0);
		RenderMesh(meshList[GEO_ISOPOD_LEG], false);
		modelStack.PopMatrix();
	}
	modelStack.PushMatrix();//left claw
	modelStack.Translate(0.9, 0.05, 0.22);
	modelStack.Rotate(30, 0, 0,1);
	RenderMesh(meshList[GEO_ISOPOD_CLAW], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();//right claw
	modelStack.Translate(0.9, 0.05, -0.22);
	modelStack.Rotate(30, 0, 0,1);
	RenderMesh(meshList[GEO_ISOPOD_CLAW], false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void SceneNightmareTrench::RenderWorld()
{
	RenderTerrain();
	RenderSkyPlane();
	RenderBoss();

	modelStack.PushMatrix();
	modelStack.Translate(playerpos.x, playerpos.y+5, playerpos.z);
	modelStack.Rotate(90 + fishRot.y, 0, 1, 0);
	modelStack.Rotate(0 + fishRot.x, 1, 0, 0);
	modelStack.Scale(15,15,15);
	RenderMesh(meshList[GEO_FISHMODEL], true);
	modelStack.PushMatrix();
	modelStack.Scale(1, 1, 1);
	modelStack.Translate(0,-0.02,-1.2);
	modelStack.Rotate(fish_tailrot, 0, 1, 0);

	RenderMesh(meshList[GEO_FISHTAIL], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	


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

			}break;
			case SeaCreature::CHIMERA:
			{
				Chimera * c = (Chimera*)*it;
				RenderChimera(c);

			}break;

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


	
}

void SceneNightmareTrench::RenderPassGPass()
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

void SceneNightmareTrench::RenderPassMain()
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
	SceneSP3::RenderParticles();
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

	//for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	//{
	//	GameObject *go = (GameObject*)*it;
	//	if (go->objectType == GameObject::SEACREATURE)
	//	{
	//		Minnow *fo = (Minnow*)*it;
	//		if (fo->active)
	//		{
	//			RenderFO(fo);
	//		}
	//	}
	//	else if (go->objectType == GameObject::PROJECTILE)
	//	{
	//		Projectile *po = (Projectile*)*it;
	//		if (po->active)
	//		{
	//			RenderPO(po);
	//		}
	//	}
	//}

	// Render the crosshair
	glUniform1i(m_parameters[U_FOG_ENABLE], 0);
	RenderMesh(meshList[GEO_AXES], false);
	RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 10.0f);
	SceneSP3::RenderMinimap();
	std::ostringstream ss;
	ss.precision(3);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 2, 3);

	//modelStack.PushMatrix();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	//set to line
	//modelStack.Translate(m_travelzonedown.m_position.x,m_travelzonedown.m_position.y,m_travelzonedown.m_position.z);
	//modelStack.Scale(m_travelzonedown.m_width,m_travelzonedown.m_height,m_travelzonedown.m_length);
	//RenderMesh(meshList[GEO_CUBE], false);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	//set back to fill
	//modelStack.PopMatrix();


	modelStack.PushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	//set to line
	modelStack.Translate(m_travelzoneup.m_position.x, m_travelzoneup.m_position.y, m_travelzoneup.m_position.z);
	modelStack.Scale(m_travelzoneup.m_width, m_travelzoneup.m_height, m_travelzoneup.m_length);
	RenderMesh(meshList[GEO_CUBE], false);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	//set back to fill
	modelStack.PopMatrix();

}

void SceneNightmareTrench::Render()
{
	// PRE RENDER PASS
	RenderPassGPass();

	// MAIN RENDER PASS
	RenderPassMain();

}

void SceneNightmareTrench::RenderChimera(Chimera* c)
{
	//float rotate = 0;
	//rotate = Math::RadianToDegree(atan2(c->vel.x, c->vel.z));
	modelStack.PushMatrix();
	modelStack.Translate(c->pos.x, c->pos.y, c->pos.z);
	modelStack.Rotate(-c->rotate, 0, 1, 0);
	modelStack.Scale(c->scale.x, c->scale.y, c->scale.z);
	RenderMesh(meshList[GEO_CHIMERA_BODY], false);

	modelStack.PushMatrix();
	modelStack.Translate(0,-0.05,0.18);
	modelStack.Rotate(c->flip[0].rotation, 0, 0, 1);
	modelStack.Rotate(20, 1, 0, 0);
	RenderMesh(meshList[GEO_CHIMERA_FFLIP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -0.05, 0.18);
	modelStack.Rotate(-c->flip[1].rotation, 0, 0, 1);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(-20, 1, 0, 0);
	RenderMesh(meshList[GEO_CHIMERA_FFLIP], false);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(0.08, -0.1, -0.3);
	modelStack.Rotate(c->flip[2].rotation, 0, 0, 1);
	RenderMesh(meshList[GEO_CHIMERA_BFLIP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-0.08, -0.1, -0.3);
	modelStack.Rotate(-c->flip[3].rotation, 0, 0, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_CHIMERA_BFLIP], false);
	modelStack.PopMatrix();




	modelStack.PopMatrix();
}

Chimera* SceneNightmareTrench::FetchChimera()
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		Chimera *go = (Chimera *)*it;
		if (!go->active)
		{
			go->active = true;
			//++m_objectCount;
			return go;
		}

	}
	for (unsigned i = 0; i < 10; ++i)
	{
		Chimera *go = new Chimera();
		go->objectType = GameObject::SEACREATURE;
		go->seaType = SeaCreature::CHIMERA;
		m_goList.push_back(go);
	}
	Chimera *go = (Chimera *)m_goList.back();
	go->active = true;
	//++m_objectCount;
	return go;
}

void SceneNightmareTrench::RenderMinimap()
{

}

void SceneNightmareTrench::Update(double dt)
{
	SceneSP3::Update(dt);


	//if ()
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject*)*it;

		if (!go->active)
			continue;


		if (go->objectType != GameObject::SEACREATURE)
			continue;
		
			SeaCreature * fo = (SeaCreature*)*it;

			
			if (fo->seaType != SeaCreature::CHIMERA)
				continue;

				
				Chimera * c = (Chimera*)*it;
				
				if (collision(c->aabb, player_box))
				{
					//do dmamage
					//c->rotate = -c->rotate;
					c->chstate = Chimera::FLEEING;

				}
				
				c->UpdateChimera(dt);
	}

    static bool trest = false;
    if (Application::IsKeyPressed('E') && !trest)
    {
        trest = true;
    }
    else if (!Application::IsKeyPressed('E') && trest)
    {
        for (int i = 0; i < 10; ++i)
        {
            Chimera*c = FetchChimera();
            c->active = true;
            c->objectType = GameObject::SEACREATURE;
            c->seaType = SeaCreature::CHIMERA;
            //p->pstate = Pufferfish::IDLE;
            c->scale.Set(20, 20, 20);
            c->pos.Set(Math::RandFloatMinMax(-1000, 1000), Math::RandFloatMinMax(200, 500), Math::RandFloatMinMax(-1000, 1000));
            c->vel.Set(0, Math::RandFloatMinMax(-20, 20), 0);
            //p->collision = hitbox2::generatehitbox(p->pos, 8, 8, 8);
            c->setHealth(200);
        }

        trest = false;
    }

   // ChimeraSpawner.CheckCount(g_ChimeraCount, g_MaxChimera);

    //if (ChimeraSpawner.getIsSpawn())
    //{
    //    Vector3 tv(Math::RandFloatMinMax(0.f, 1000.f), Math::RandFloatMinMax(0, 1000.f), Math::RandFloatMinMax(0.f, 1000.f));
    //    if (!terraincollision(tv, m_heightMap[SharedData::GetInstance()->SD_CurrentArea]))
    //    {
    //        Chimera*c = FetchChimera();
    //        c->active = true;
    //        c->objectType = GameObject::SEACREATURE;
    //        c->seaType = SeaCreature::CHIMERA;
    //        //p->pstate = Pufferfish::IDLE;
    //        c->scale.Set(20, 20, 20);
    //        c->pos.Set(tv.x, tv.y, tv.z);
    //        c->vel.Set(0, Math::RandFloatMinMax(-20, 20), 0);
    //        //p->collision = hitbox2::generatehitbox(p->pos, 8, 8, 8);
    //        c->setHealth(200);
    //    }
    //}

	isopod->UpdateIsopod(dt,m_heightMap[4]);

}




void SceneNightmareTrench::Exit()
{
	SceneSP3::Exit();
}