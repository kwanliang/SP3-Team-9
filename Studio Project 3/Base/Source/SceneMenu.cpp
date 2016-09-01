#include "SceneMenu.h"
#include "GL\glew.h"

#include <sstream>
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <iostream>
SceneMenu::SceneMenu()
{

}

SceneMenu::~SceneMenu()
{

}

void SceneMenu::Init()
{
    // Black background
    glClearColor(0.0f, 0.8f, 0.7f, 0.0f);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);

    m_programID = LoadShaders("Shader//Shadow.vertexshader", "Shader//Shadow.fragmentshader");

    // Get a handle for our uniform
    m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
    //m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
    //m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
    m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
    m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
    m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
    m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
    m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
    m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");

    // Get a handle for our "colorTexture" uniform
    m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[0]");
    m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture[0]");
    m_parameters[U_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_programID, "colorTextureEnabled[1]");
    m_parameters[U_COLOR_TEXTURE1] = glGetUniformLocation(m_programID, "colorTexture[1]");

    m_parameters[U_TEXTURE_BLEND_ENABLE] = glGetUniformLocation(m_programID, "blendParam.blendEnabled");
    m_parameters[U_TEXTURE_BLEND_FACTOR] = glGetUniformLocation(m_programID, "blendParam.blendFactor");

    // Use our shader
    glUseProgram(m_programID);

    meshList[GEO_TTITLE] = MeshBuilder::GenerateQuad("title", Color(1, 0, 0), 2);
    meshList[GEO_TTITLE]->textureID = LoadTGA("Image//title.tga");

    meshList[GEO_TMENU] = MeshBuilder::GenerateQuad("menu screen", Color(1, 0, 0), 2);
    meshList[GEO_TMENU]->textureID = LoadTGA("Image//long_title.tga");

    meshList[GEO_TSTART] = MeshBuilder::GenerateQuad("start", Color(1, 0, 0), 2);
    meshList[GEO_TSTART]->textureID = LoadTGA("Image//start.tga");

    meshList[GEO_TQUIT] = MeshBuilder::GenerateQuad("quit", Color(1, 0, 0), 2);
    meshList[GEO_TQUIT]->textureID = LoadTGA("Image//quit.tga");

	meshList[GEO_MFISH] = MeshBuilder::GenerateQuad("fish", Color(1, 0, 0), 2);
	meshList[GEO_MFISH]->textureID = LoadTGA("Image//menu_fish.tga");

    select = START;
	xscroll = 170;
	mfish_x = -81;
}

void SceneMenu::Render()
{
	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	

	RenderMeshIn2D(meshList[GEO_TMENU], false, 250, 75, xscroll, 0, 0);RenderMeshIn2D(meshList[GEO_MFISH], false, 30, 30, mfish_x, 0, 0);
	RenderMeshIn2D(meshList[GEO_TTITLE], false, 30, 30, 0, 30);
	switch (select)
	{
	case START:
	{
		RenderMeshIn2D(meshList[GEO_TSTART], false, 40, 8, 0, -15);
		RenderMeshIn2D(meshList[GEO_TQUIT], false, 20, 4, 0, -40);
		break;
	}
	case QUIT:
	{
		RenderMeshIn2D(meshList[GEO_TSTART], false, 20, 4, 0, -15);
		RenderMeshIn2D(meshList[GEO_TQUIT], false, 40, 8, 0, -40);
		break;
	}
    }

}

void SceneMenu::Update(double dt)
{
    if (Application::IsKeyPressed('W') || Application::IsKeyPressed(VK_UP))
        select = SELECT::START;
    else if (Application::IsKeyPressed('S') || Application::IsKeyPressed(VK_DOWN))
        select = SELECT::QUIT;

    if (select == SELECT::START && Application::IsKeyPressed(VK_RETURN))
    {
        Application::sceneManager->SetPreviousScene(Application::sceneManager->GetCurrentScene());
        Application::sceneManager->SetCurrentScene(Application::sceneManager->GetLoadingScreen());
    }
    else if (select == SELECT::QUIT && Application::IsKeyPressed(VK_RETURN))
    {
        SharedData::GetInstance()->SD_QuitGame = true;
    }


	if (Application::IsKeyPressed('Z'))
	{
		val += 10.f * (float)dt;
		std::cout << val << std::endl;
	}
	if (Application::IsKeyPressed('X'))
	{
		val -= 10.f * (float)dt;
		std::cout << val << std::endl;
	}
	//xscroll = val;

	if (xscroll > -81)
		xscroll -= dt * 40;
	else
		xscroll = 170;



	mfish_x -= dt*80;

	if (mfish_x < -170)
		mfish_x = 100;

}


void SceneMenu::Exit()
{
    delete meshList[GEO_TTITLE];
    delete meshList[GEO_TMENU];
    delete meshList[GEO_TSTART];
    delete meshList[GEO_TQUIT];
	delete meshList[GEO_MFISH];

    glDeleteProgram(m_programID);
    glDeleteVertexArrays(1, &m_vertexArrayID);
}