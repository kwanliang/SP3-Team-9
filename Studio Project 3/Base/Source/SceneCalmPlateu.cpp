#include "SceneCalmPlateu.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

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
	

	for (int i = 0; i < 10; i++)
	{
		Pufferfish *p = FetchPuffer();
		p->active = true;
		p->objectType = GameObject::SEACREATURE;
		p->seaType = SeaCreature::PUFFER;
		p->pstate = Pufferfish::IDLE;
		p->scale.Set(5, 5, 5);
		p->pos.Set(0,500,0);
		p->vel.Set(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10));
		p->collision = hitbox2::generatehitbox(p->pos, 8, 8, 8);
		p->setHealth(200);
	}

	m_travelzonedown = hitbox::generatehitbox(Vector3(1386, 295, 5.8), 200, 600, 600, 0);
	m_travelzoneup = hitbox::generatehitbox(Vector3(-1258, 389, -1221), 500, 700, 500, 0);

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

void SceneCalmPlateu::RenderParticles()
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

void SceneCalmPlateu::RenderWorld()
{
	RenderTerrain();
	RenderSkyPlane();
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

			}break;
			case SeaCreature::PUFFER:
			{
				Pufferfish * p = (Pufferfish*)*it;
				RenderPuffer(p);
				
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

	glUniform1i(m_parameters[U_FOG_ENABLE], 0);
	// Render the crosshair
	RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 10.0f);
	RenderMesh(meshList[GEO_AXES], false);
	SceneSP3::RenderMinimap();
	//std::ostringstream ss;
	//ss.precision(3);
	//ss << "FPS: " << fps;
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 2, 3);

	modelStack.PushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	//set to line
	modelStack.Translate(m_travelzonedown.m_position.x,m_travelzonedown.m_position.y,m_travelzonedown.m_position.z);
	modelStack.Scale(m_travelzonedown.m_width,m_travelzonedown.m_height,m_travelzonedown.m_length);
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
	RenderMesh(meshList[GEO_PUFFER],false);
	modelStack.PopMatrix();
		
}

void SceneCalmPlateu::RenderMinimap()
{

}

void SceneCalmPlateu::Update(double dt)
{
	SceneSP3::Update(dt);
	UpdatePuffer(dt);
}

void SceneCalmPlateu::UpdatePuffer(double dt)
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active)
			continue;

		if (go->objectType != GameObject::SEACREATURE)
			continue;

		Pufferfish *p = (Pufferfish *)*it;

		if (p->seaType != SeaCreature::PUFFER)
			continue;


		hitbox2::updatehitbox(p->collision, p->pos);
		switch (p->pstate)
		{
		case Pufferfish::IDLE:
		{
			p->pos += p->vel.Normalize() * dt * 10;
			
			//check bullet collision
			for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
			{
				GameObject *b = (GameObject *)*it;
				if (b->objectType == GameObject::PROJECTILE && b->active)
				{
					
					Vector3 displacement = p->pos - b->pos;
					if (displacement.LengthSquared() <  p->scale.z + b->scale.z + 50)
					{
						p->scale = Vector3(10, 10, 10);
						//p->collision.m_height
						p->pstate = Pufferfish::ENRAGED;
						std::cout << " hit" << std::endl;
						break;
					}
				}
			}
			p->pos += p->vel.Normalize() * dt * 10;

		}break;
		case Pufferfish::ENRAGED:
		{
			Vector3 displacement = playerpos - p->pos;
			if (displacement.LengthSquared() > 400*400)
			{
				p->vel *= -1.f;
				p->scale = Vector3(5, 5, 5);
				p->pstate = Pufferfish::IDLE;
			}
			p->vel = displacement.Normalize();
			if (collision(p->collision, player_box))
			{
				//p->vel *= -1.f;
				if (fishVel.Length() < 3)
					fishVel = Vector3(5, 5, 5);

				fishVel *= -5.f;
				walkCam.Move(fishVel * (float)dt);
				playerpos = walkCam.GetPos() + Vector3(0, 80, 0);
			}

			p->pos += p->vel * dt * 20;
			
		}break;
		}
		
		if (terraincollision(p->collision, m_heightMap[SharedData::GetInstance()->SD_CurrentArea]))//check collision;
		{
			p->vel *= -5.f;
			p->pos.y += 4;
		}
		if (p->pos.y > 650)//heigt limit
		{
			p->pos.y -= 1;
			p->vel.y = -p->vel.y;
		}
	
		// Cap velocity
		if (p->vel.x > 20)
			p->vel.x = 20;
		if (p->vel.y > 20)
			p->vel.y = 20;
		if (p->vel.z > 20)
			p->vel.z = 20;
		if (p->vel.x < -20)
			p->vel.x = -20;
		if (p->vel.y < -20)
			p->vel.y = -20;
		if (p->vel.z < -20)
			p->vel.z = -20;

	}
}

Pufferfish*  SceneCalmPlateu::FetchPuffer()
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		Pufferfish *go = (Pufferfish *)*it;
		if (!go->active)
		{
			go->active = true;
			//++m_objectCount;
			return go;
		}

	}
	for (unsigned i = 0; i < 10; ++i)
	{
		Pufferfish *go = new Pufferfish();
		go->objectType = GameObject::SEACREATURE;
		go->seaType = SeaCreature::PUFFER;
		m_goList.push_back(go);
	}
	Pufferfish *go = (Pufferfish *)m_goList.back();
	go->active = true;
	//++m_objectCount;
	return go;
}

void SceneCalmPlateu::Exit()
{
	SceneSP3::Exit();
}