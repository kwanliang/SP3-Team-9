#include "SceneLoading.h"
#include "GL\glew.h"

#include <sstream>
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"

SceneLoading::SceneLoading()
{

}

SceneLoading::~SceneLoading()
{

}

void SceneLoading::Init()
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

    meshList[GEO_TLOADING] = MeshBuilder::GenerateQuad("loading screen", Color(0, 0, 0), 2);
	meshList[GEO_TLOADING]->textureID = LoadTGA("Image//long_title.tga");

	meshList[GEO_VACUUM] = MeshBuilder::GenerateQuad("vacuum", Color(1, 0, 0), 1.f);
	meshList[GEO_VACUUM]->textureID = LoadTGA("Image//vacuum.tga");
}

void SceneLoading::Render()
{
	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	RenderMeshIn2D(meshList[GEO_TLOADING], false, 250, 75);
	RenderMeshIn2D(meshList[GEO_VACUUM], false, 10.0f, 10.0f, 0, 0, 0 );
}

void SceneLoading::Update(double dt)
{
    Application::sceneManager->LoadScene();
}

void SceneLoading::Exit()
{
    delete meshList[GEO_TLOADING];

    glDeleteProgram(m_programID);
    glDeleteVertexArrays(1, &m_vertexArrayID);
}