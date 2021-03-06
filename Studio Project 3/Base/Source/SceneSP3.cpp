#include "SceneSP3.h"
#include "GL\glew.h"

#include <iterator>
#include <sstream>
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "LoadFontData.h"

using std::cout;
using std::endl;

//SpriteAnimation *SPRITENAME;

SceneSP3::SceneSP3()
{
}

SceneSP3::~SceneSP3()
{
}

void SceneSP3::Init()
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

    // Frame buffers

    // Shadow
    m_lightDepthFBO.Init(4096, 4096);

    m_gPassShaderID = LoadShaders("Shader//GPass.vertexshader", "Shader//GPass.fragmentshader");
    m_parameters[U_LIGHT_DEPTH_MVP_GPASS] = glGetUniformLocation(m_gPassShaderID, "lightDepthMVP");

    m_programID = LoadShaders("Shader//Shadow.vertexshader", "Shader//Shadow.fragmentshader");
    m_parameters[U_LIGHT_DEPTH_MVP] = glGetUniformLocation(m_programID, "lightDepthMVP");

    m_parameters[U_SHADOW_MAP] = glGetUniformLocation(m_programID, "shadowMap");

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
    m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
    m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

    // Directional light for shadows
    m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
    m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
    m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
    m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
    m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
    m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
    m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
    m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
    m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
    m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
    m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

    // Directional light for scene
    m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
    m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
    m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
    m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
    m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
    m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
    m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
    m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
    m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
    m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
    m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

    // Spotlight campfire
    m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
    m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
    m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
    m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
    m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
    m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
    m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
    m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
    m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
    m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
    m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");

    // Get a handle for our "colorTexture" uniform
    m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[0]");
    m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture[0]");
    m_parameters[U_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_programID, "colorTextureEnabled[1]");
    m_parameters[U_COLOR_TEXTURE1] = glGetUniformLocation(m_programID, "colorTexture[1]");
    // Get a handle for our "textColor" uniform
    m_parameters[U_IS_GUI] = glGetUniformLocation(m_programID, "isGUI");
    m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

    // Get a handle for our "fog" uniform
    m_parameters[U_FOG_COLOR] = glGetUniformLocation(m_programID, "fogParam.color");
    m_parameters[U_FOG_START] = glGetUniformLocation(m_programID, "fogParam.start");
    m_parameters[U_FOG_END] = glGetUniformLocation(m_programID, "fogParam.end");
    m_parameters[U_FOG_THICKNESS] = glGetUniformLocation(m_programID, "fogParam.thickness");
    m_parameters[U_FOG_DENSITY] = glGetUniformLocation(m_programID, "fogParam.density");
    m_parameters[U_FOG_TYPE] = glGetUniformLocation(m_programID, "fogParam.type");
    m_parameters[U_FOG_ENABLE] = glGetUniformLocation(m_programID, "fogParam.enabled");

    // Get a handle for our "shadow" uniform
    m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[0]");
    m_parameters[U_SHADOW_COLOR_TEXTURE] = glGetUniformLocation(m_gPassShaderID, "colorTexture[0]");
    m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[1]");
    m_parameters[U_SHADOW_COLOR_TEXTURE1] = glGetUniformLocation(m_gPassShaderID, "colorTexture[1]");
    m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED2] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[2]");
    m_parameters[U_SHADOW_COLOR_TEXTURE2] = glGetUniformLocation(m_gPassShaderID, "colorTexture[2]");

    m_parameters[U_TEXTURE_BLEND_ENABLE] = glGetUniformLocation(m_programID, "blendParam.blendEnabled");
    m_parameters[U_TEXTURE_BLEND_FACTOR] = glGetUniformLocation(m_programID, "blendParam.blendFactor");

    // Use our shader
    glUseProgram(m_programID);

    glUniform1i(m_parameters[U_NUMLIGHTS], 1);
    glUniform1i(m_parameters[U_IS_GUI], 0);

    glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
    glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
    glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
    glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
    glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
    glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
    glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
    glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
    glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

    glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
    glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
    glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
    glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
    glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
    glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
    glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
    glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
    glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

    glUniform1i(m_parameters[U_LIGHT2_TYPE], lights[2].type);
    glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &lights[2].color.r);
    glUniform1f(m_parameters[U_LIGHT2_POWER], lights[2].power);
    glUniform1f(m_parameters[U_LIGHT2_KC], lights[2].kC);
    glUniform1f(m_parameters[U_LIGHT2_KL], lights[2].kL);
    glUniform1f(m_parameters[U_LIGHT2_KQ], lights[2].kQ);
    glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], lights[2].cosCutoff);
    glUniform1f(m_parameters[U_LIGHT2_COSINNER], lights[2].cosInner);
    glUniform1f(m_parameters[U_LIGHT2_EXPONENT], lights[2].exponent);

    Color fogColor(0.0f, 0.8f, 0.7f);
    glUniform3fv(m_parameters[U_FOG_COLOR], 1, &fogColor.r);
    glUniform1f(m_parameters[U_FOG_START], 10);
    glUniform1f(m_parameters[U_FOG_END], 500);
    glUniform1f(m_parameters[U_FOG_THICKNESS], 1.f);
    glUniform1f(m_parameters[U_FOG_DENSITY], 0.0f);
    glUniform1f(m_parameters[U_FOG_TYPE], 1);
    glUniform1f(m_parameters[U_FOG_ENABLE], 1);


    // camera.Init(Vector3(0, 70, 10), Vector3(0, 70, 0), Vector3(0, 1, 0));
    walkCam.Init(
        Vector3(-852, 544, -699),
        Vector3(0, 0, -10),
        Vector3(0, 1, 0),
        100
        );
    currentCam = &walkCam;
    m_travelzonedown = hitbox::generatehitbox(Vector3(52, 579, 1310), 600, 500, 600, 0);
    for (int i = 0; i < NUM_GEOMETRY; ++i)
    {
        meshList[i] = NULL;
    }

    meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 1, 1), 1.f);
    meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");//, 1000, 1000, 1000);
    meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateQuad("minimap", Color(0, 0, 0), 2);
    meshList[GEO_CROSSHAIR]->textureID = LoadTGA("Image//crosshair.tga");
    meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
    meshList[GEO_TEXT]->textureID = LoadTGA("Image//sfont2.tga");
    meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);

    meshList[GEO_MINIMAP] = MeshBuilder::GenerateQuad("minimap", Color(1, 1, 1), 2);
    meshList[GEO_MINIMAP]->textureID = LoadTGA("Image//minimap.tga");

	meshList[GEO_MINIMAP_AVATAR] = MeshBuilder::GenerateQuad("minimap_avatar", Color(1, 1, 1), 2);
	meshList[GEO_MINIMAP_AVATAR]->textureID = LoadTGA("Image//minimap_avatar.tga");
	meshList[GEO_MINIMAP_MINNOW] = MeshBuilder::GenerateSphere("minimap_minnow", Color(0.5f, 1, 1), 16, 16);
	meshList[GEO_MINIMAP_CUTTLE] = MeshBuilder::GenerateSphere("minimap_cuttle", Color(1, 1, 1), 16, 16);
	meshList[GEO_MINIMAP_CRAB] = MeshBuilder::GenerateSphere("minimap_crab", Color(1, 0.75f, 0.75f), 16, 16);
	meshList[GEO_MINIMAP_GHOSTSHARK] = MeshBuilder::GenerateSphere("minimap_ghostshark", Color(0.1, 0.9f, 0.4f), 16, 16, 1.2f);
	meshList[GEO_MINIMAP_PUFFER] = MeshBuilder::GenerateSphere("minimap_puffer", Color(0.9f, 0.9f, 0.2f), 16, 16);
	meshList[GEO_MINIMAP_BOSS] = MeshBuilder::GenerateSphere("minimap_boss", Color(1, 0.25f, 0.25f), 16, 16);
	meshList[GEO_MINIMAP_ARROW_DOWN] = MeshBuilder::GenerateQuad("minimap_arrowdown", Color(0, 0.5f, 0), 3.5f);
	meshList[GEO_MINIMAP_ARROW_DOWN]->textureID = LoadTGA("Image//minimap_arrow_d.tga");
	meshList[GEO_MINIMAP_ARROW_UP] = MeshBuilder::GenerateQuad("minimap_arrowup", Color(0, 1.f, 0), 3.5f);
	meshList[GEO_MINIMAP_ARROW_UP]->textureID = LoadTGA("Image//minimap_arrow_u.tga");

    meshList[GEO_FISHMODEL] = MeshBuilder::GenerateOBJ("fishModel", "Models//OBJ//rcfish.obj");
    meshList[GEO_FISHMODEL]->textureArray[0] = LoadTGA("Image//RCfish.tga");
    meshList[GEO_FISHTAIL] = MeshBuilder::GenerateOBJ("fishModel", "Models//OBJ//rctail.obj");

    meshList[GEO_MINNOW] = MeshBuilder::GenerateOBJ("minnow", "Models//OBJ//minnow.obj");
    meshList[GEO_MINNOW]->textureArray[0] = LoadTGA("Image//minnow.tga");

    meshList[GEO_LASER] = MeshBuilder::GenerateOBJ("beam", "Models//OBJ//laser.obj");
    meshList[GEO_LASER]->textureArray[0] = LoadTGA("Image//laser.tga");

    meshList[GEO_CORAL] = MeshBuilder::GenerateOBJ("coral health", "Models//OBJ//coral_1.obj");
    meshList[GEO_CORAL]->textureArray[0] = LoadTGA("Image//coral_1.tga");

    meshList[GEO_CORAL2] = MeshBuilder::GenerateOBJ("coral stamina", "Models//OBJ//coral_2.obj");
    meshList[GEO_CORAL2]->textureArray[0] = LoadTGA("Image//coral_2.tga");

	// Captured
	meshList[GEO_CAP_MINNOW] = MeshBuilder::GenerateOBJ("minnow", "Models//OBJ//r_minow.obj");
	meshList[GEO_CAP_MINNOW]->textureArray[0] = LoadTGA("Image//rminnow.tga");

	meshList[GEO_CAP_FCRABBODY] = MeshBuilder::GenerateOBJ("crabbody", "Models//OBJ//r_fcrab_body.obj");
	meshList[GEO_CAP_FCRABBODY]->textureArray[0] = LoadTGA("Image//rfcrab.tga");
	meshList[GEO_CAP_FCRABLEG] = MeshBuilder::GenerateOBJ("crableg", "Models//OBJ//r_fcrab_leg.obj");
	meshList[GEO_CAP_FCRABLEG]->textureArray[0] = LoadTGA("Image//rfcrab.tga");
	meshList[GEO_CAP_FCRABCLAW] = MeshBuilder::GenerateOBJ("crabclaw", "Models//OBJ//r_fcrab_Lclaw.obj");
	meshList[GEO_CAP_FCRABCLAW]->textureArray[0] = LoadTGA("Image//rfcrab.tga");

	meshList[GEO_CAP_PUFFER] = MeshBuilder::GenerateOBJ("puffer", "Models//OBJ//r_puffer.obj");
	meshList[GEO_CAP_PUFFER]->textureArray[0] = LoadTGA("Image//rpuffer.tga");

	meshList[GEO_CAP_CUTTLE] = MeshBuilder::GenerateOBJ("cuttle", "Models//OBJ//r_cuttlefish.obj");
	meshList[GEO_CAP_CUTTLE]->textureArray[0] = LoadTGA("Image//rcuttle.tga");

	meshList[GEO_CAP_CHIMERA_BODY] = MeshBuilder::GenerateOBJ("chimera", "Models//OBJ//r_ghostshark_body.obj");
	meshList[GEO_CAP_CHIMERA_BODY]->textureArray[0] = LoadTGA("Image//rghostshark.tga");
	meshList[GEO_CAP_CHIMERA_FFLIP] = MeshBuilder::GenerateOBJ("chimera", "Models//OBJ//r_ghostshark_Fflipper.obj");
	meshList[GEO_CAP_CHIMERA_FFLIP]->textureArray[0] = LoadTGA("Image//rghostshark.tga");
	meshList[GEO_CAP_CHIMERA_BFLIP] = MeshBuilder::GenerateOBJ("chimera", "Models//OBJ//r_ghostshark_Bflipper.obj");
	meshList[GEO_CAP_CHIMERA_BFLIP]->textureArray[0] = LoadTGA("Image//rghostshark.tga");


	meshList[GEO_VORTEX] = MeshBuilder::GenerateOBJ("cuttle", "Models//OBJ//vortex.obj");

    meshList[GEO_BALL] = MeshBuilder::GenerateSphere("ball", Color(0, 0, 0), 16, 16, 1.f);
    meshList[GEO_BALL2] = MeshBuilder::GenerateSphere("ball", Color(1, 0, 0), 16, 16, 1.f);

    // Particles
    meshList[GEO_BUBBLE] = MeshBuilder::GenerateQuad("bubble", Color(1, 0, 0), 1.f);
    meshList[GEO_BUBBLE]->textureArray[0] = LoadTGA("Image//bubble.tga");

    meshList[GEO_VACUUM] = MeshBuilder::GenerateQuad("vacuum", Color(1, 0, 0), 1.f);
    meshList[GEO_VACUUM]->textureArray[0] = LoadTGA("Image//vacuum.tga");

    // HUD
    meshList[GEO_HUD_HEALTHBAR] = MeshBuilder::GenerateQuad("health bar hud", Color(1, 0, 0), 2.f);
    meshList[GEO_HUD_HEALTHBAR]->textureID = LoadTGA("Image//boss_hp_back.tga");
    meshList[GEO_HUD_BOSSHEALTH] = MeshBuilder::GenerateQuad("boss health hud", Color(1, 0, 0), 2.f);
    meshList[GEO_HUD_BOSSHEALTH]->textureID = LoadTGA("Image//boss_hp_front.tga");

    meshList[GEO_PLAYERHEALTH] = MeshBuilder::GenerateLine("player health", Color(1.f, 0.7f, 0.7f), Vector3(0.85f, 0, 0), Vector3(0.8f, 0, 0));
    meshList[GEO_PLAYERSTAMINA] = MeshBuilder::GenerateLine("player Stamina", Color(0.7f, 0.7f, 1.f), Vector3(0.85f, 0, 0), Vector3(0.8f, 0, 0));

    meshList[GEO_TVICTORY] = MeshBuilder::GenerateQuad("hud victory", Color(1, 0, 0), 2.f);
    meshList[GEO_TVICTORY]->textureID = LoadTGA("Image//victory.tga");
    meshList[GEO_TCREDITS] = MeshBuilder::GenerateQuad("hud credits", Color(1, 0, 0), 2.f);
    meshList[GEO_TCREDITS]->textureID = LoadTGA("Image//credits.tga");

    // Death Screen
    meshList[GEO_TBORDER] = MeshBuilder::GenerateQuad("border", Color(1, 0, 0), 2);
    meshList[GEO_TBORDER]->textureID = LoadTGA("Image//border.tga");
    meshList[GEO_TLAYER] = MeshBuilder::GenerateQuad("layer", Color(1, 0, 0), 2);
    meshList[GEO_TLAYER]->textureID = LoadTGA("Image//layer.tga");
    meshList[GEO_TDIED] = MeshBuilder::GenerateQuad("die title", Color(1, 0, 0), 2);
    meshList[GEO_TDIED]->textureID = LoadTGA("Image//died.tga");
    meshList[GEO_TRESPAWN] = MeshBuilder::GenerateQuad("respawn select", Color(1, 0, 0), 2);
    meshList[GEO_TRESPAWN]->textureID = LoadTGA("Image//respawn.tga");
    meshList[GEO_TMENU] = MeshBuilder::GenerateQuad("menu select", Color(1, 0, 0), 2);
    meshList[GEO_TMENU]->textureID = LoadTGA("Image//tomenu.tga");
    meshList[GEO_TQUIT] = MeshBuilder::GenerateQuad("quit select", Color(1, 0, 0), 2);
    meshList[GEO_TQUIT]->textureID = LoadTGA("Image//quit2.tga");

    meshList[GEO_MINNOW_HUD] = MeshBuilder::GenerateQuad("hud minnow", Color(1, 0, 0), 2);
    meshList[GEO_MINNOW_HUD]->textureID = LoadTGA("Image//minnow_icon.tga");
    meshList[GEO_CUTTLE_HUD] = MeshBuilder::GenerateQuad("hud cuttle", Color(1, 0, 0), 2);
    meshList[GEO_CUTTLE_HUD]->textureID = LoadTGA("Image//cuttle_icon.tga");
    meshList[GEO_CRAB_HUD] = MeshBuilder::GenerateQuad("hud crab", Color(1, 0, 0), 2);
    meshList[GEO_CRAB_HUD]->textureID = LoadTGA("Image//crab_icon.tga");
    meshList[GEO_CHIMERA_HUD] = MeshBuilder::GenerateQuad("hud chimera", Color(1, 0, 0), 2);
    meshList[GEO_CHIMERA_HUD]->textureID = LoadTGA("Image//chimera_icon.tga");
    meshList[GEO_PUFFER_HUD] = MeshBuilder::GenerateQuad("hud puffer", Color(1, 0, 0), 2);
    meshList[GEO_PUFFER_HUD]->textureID = LoadTGA("Image//puffer_icon.tga");

    meshList[GEO_CORAL_HUD] = MeshBuilder::GenerateQuad("hud coral", Color(1, 0, 0), 2);
    meshList[GEO_CORAL_HUD]->textureID = LoadTGA("Image//heal_icon.tga");
    meshList[GEO_CORAL2_HUD] = MeshBuilder::GenerateQuad("hud coral 2", Color(1, 0, 0), 2);
    meshList[GEO_CORAL2_HUD]->textureID = LoadTGA("Image//boost_icon.tga");

    fontData = LoadFontData("Image//FontData.csv");

    // Instruction Screen
    meshList[GEO_INSTRUCTION] = MeshBuilder::GenerateQuad("instructions", Color(1, 0, 0), 2.f);
    meshList[GEO_INSTRUCTION]->textureID = LoadTGA("Image//instructions.tga");
    meshList[GEO_INSTRUCTION2] = MeshBuilder::GenerateQuad("instructions 2", Color(1, 0, 0), 2.f);
    meshList[GEO_INSTRUCTION2]->textureID = LoadTGA("Image//instructions2.tga");

    // Pause Screen
    meshList[GEO_TPAUSE] = MeshBuilder::GenerateQuad("pause", Color(1, 0, 0), 2.f);
    meshList[GEO_TPAUSE]->textureID = LoadTGA("Image//pause.tga");
    meshList[GEO_TRESUME] = MeshBuilder::GenerateQuad("resume", Color(1, 0, 0), 2.f);
    meshList[GEO_TRESUME]->textureID = LoadTGA("Image//resume.tga");

    // Hitbox generation
	playerpos = Vector3(0, 0, 0);
	player_box = hitbox2::generatehitbox(playerpos, 10, 10, 10);
	fish_tailrot = 0;
	fish_tailmax = false;
	m_spCount = 0;
	m_fireRate = 0;
	vrotate = 0;

    sliderTranslate = -100.f;
    fogThickness = 0.f;
    blendFactor = 0.f;

    bLightEnabled = true;

    m_particleCount = 0;
    m_gravity.Set(0, -9.8f, 0);
	
	walkCam.yOffset = 100;

    skipper = new Skipper();
    skipper->active = true;
    skipper->objectType = GameObject::PLAYER;
    skipper->setHealth(500);
    skipper->scale.Set(1, 1, 1);
    skipper->pos.Set(playerpos.x, playerpos.y, playerpos.z);
    skipper->vel.Set(fishVel.x, fishVel.y, fishVel.z);
    skipper->setDamage(10);
    skipper->setBaseDamage(10);
	skipper->setTarget(skipper);
    skipper->setTimerReceieveDamage(0.0);
    skipper->setIsDead(false);
    skipper->setHealthPackCount(0);
    skipper->setStaminaPackCount(0);
    if (SharedData::GetInstance()->SD_PlayerHealth != 0)
        skipper->setHealth(SharedData::GetInstance()->SD_PlayerHealth);
    seaList.push_back(skipper);
    SharedData::GetInstance()->SD_IsImmobile = false;

    t_stuckPrevention = 0.0;

    DeathSelect = DEATHSELECT::RESPAWN;

	for (unsigned i = 0; i < 4; ++i) for (unsigned j = 0; j < 255; ++j) keyStates[i][j] = false;

	isGamePaused = false;
	pauseChoice = 0;
	g_MinnowCount = 0;
	g_MinnowLeaderCount = 0;
	g_ChimeraCount = 0;
	g_CuttlefishCount = 0;
	g_FCrabCount = 0;
	g_PufferfishCount = 0;
}

Minnow* SceneSP3::FetchMinnow()
{
    for (std::vector<GameObject *>::iterator it = seaList.begin(); it != seaList.end(); ++it)
    {
        Minnow *go = (Minnow *)*it;
        if (!go->active)
        {
            go->active = true;
            go->objectType = GameObject::SEACREATURE;
            go->seaType = SeaCreature::MINNOW;
            return go;
        }
    }
    for (unsigned i = 0; i < 10; ++i)
    {
        Minnow *go = new Minnow();
        go->objectType = GameObject::SEACREATURE;
        go->seaType = SeaCreature::MINNOW;
        seaList.push_back(go);
    }
    Minnow *go = (Minnow *)seaList.back();
    go->active = true;
    return go;
}

Pufferfish* SceneSP3::FetchPuffer()
{
    for (std::vector<GameObject *>::iterator it = seaList.begin(); it != seaList.end(); ++it)
    {
        Pufferfish *go = (Pufferfish *)*it;
        if (!go->active && go->seaType == SeaCreature::PUFFER)
        {
            go->objectType = GameObject::SEACREATURE;
            go->seaType = SeaCreature::PUFFER;
            go->active = true;
            return go;
        }
    }
    for (unsigned i = 0; i < 10; ++i)
    {
        Pufferfish *go = new Pufferfish();
        go->objectType = GameObject::SEACREATURE;
        go->seaType = SeaCreature::PUFFER;
        seaList.push_back(go);
    }
    Pufferfish *go = (Pufferfish *)seaList.back();
    go->active = true;
    return go;
}

Fcrab* SceneSP3::FetchFCrab()
{
    for (std::vector<GameObject *>::iterator it = seaList.begin(); it != seaList.end(); ++it)
    {
        Fcrab *go = (Fcrab *)*it;
        if (!go->active && go->seaType == SeaCreature::FCRAB)
        {
            go->objectType = GameObject::SEACREATURE;
            go->seaType = SeaCreature::FCRAB;
            go->active = true;
            return go;
        }
    }
    for (unsigned i = 0; i < 10; ++i)
    {
        Fcrab *go = new Fcrab();
        go->objectType = GameObject::SEACREATURE;
        go->seaType = SeaCreature::FCRAB;
        seaList.push_back(go);
    }
    Fcrab *go = (Fcrab *)seaList.back();
    go->active = true;
    return go;
}

Cuttlefish* SceneSP3::FetchCuttle()
{
    for (std::vector<GameObject *>::iterator it = seaList.begin(); it != seaList.end(); ++it)
    {
        Cuttlefish *go = (Cuttlefish *)*it;
        if (!go->active && go->seaType == SeaCreature::CUTTLE)
        {
            go->objectType = GameObject::SEACREATURE;
            go->seaType = SeaCreature::CUTTLE;
            go->active = true;
            return go;
        }
    }
    for (unsigned i = 0; i < 10; ++i)
    {
        Cuttlefish *go = new Cuttlefish();
        go->objectType = GameObject::SEACREATURE;
        go->seaType = SeaCreature::CUTTLE;
        seaList.push_back(go);
    }
    Cuttlefish *go = (Cuttlefish *)seaList.back();
    go->active = true;
    return go;
}

Chimera* SceneSP3::FetchChimera()
{
    for (std::vector<GameObject *>::iterator it = seaList.begin(); it != seaList.end(); ++it)
    {
        Chimera *go = (Chimera *)*it;
        if (!go->active && go->seaType == SeaCreature::CHIMERA)
        {
            go->objectType = GameObject::SEACREATURE;
            go->seaType = SeaCreature::CHIMERA;
            go->active = true;
            return go;
        }
    }
    for (unsigned i = 0; i < 10; ++i)
    {
        Chimera *go = new Chimera();
        go->objectType = GameObject::SEACREATURE;
        go->seaType = SeaCreature::CHIMERA;
        seaList.push_back(go);
    }
    Chimera *go = (Chimera *)seaList.back();
    go->active = true;
    return go;
}

Drone* SceneSP3::FetchDrone()
{
	for (std::vector<GameObject *>::iterator it = seaList.begin(); it != seaList.end(); ++it)
    {
        Drone *go = (Drone *)*it;
        if (!go->active && go->seaType == SeaCreature::DRONE)
        {
            go->objectType = GameObject::SEACREATURE;
            go->seaType = SeaCreature::DRONE;
            go->active = true;
            return go;
        }
    }
    for (unsigned i = 0; i < 10; ++i)
    {
        Drone *go = new Drone();
        go->objectType = GameObject::SEACREATURE;
        go->seaType = SeaCreature::DRONE;
		seaList.push_back(go);
    }
	Drone *go = (Drone *)seaList.back();
    go->active = true;
    return go;
}

Coral* SceneSP3::FetchCoral()
{
    for (std::vector<GameObject *>::iterator it = coralList.begin(); it != coralList.end(); ++it)
    {
        Coral *go = (Coral *)*it;
        if (!go->active && GameObject::CORAL)
        {
            go->objectType = GameObject::CORAL;
            go->active = true;
            return go;
        }
    }
    for (unsigned i = 0; i < 10; ++i)
    {
        Coral *go = new Coral();
        go->objectType = GameObject::CORAL;
        coralList.push_back(go);
    }
    Coral *go = (Coral *)coralList.back();
    go->active = true;
    return go;
}

Projectile* SceneSP3::FetchPO()
{
    for (std::vector<GameObject *>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
    {
        Projectile *go = (Projectile *)*it;
        if (!go->active && go->objectType == GameObject::PROJECTILE)
        {
            go->active = true;
            return go;
        }
    }
    for (unsigned i = 0; i < 10; ++i)
    {
        Projectile *go = new Projectile();
        go->objectType = GameObject::PROJECTILE;
        projectileList.push_back(go);
    }
    Projectile *go = (Projectile *)projectileList.back();
    go->active = true;
    return go;
}

DamageText* SceneSP3::FetchTO()
{
    for (std::vector<DamageText *>::iterator it = m_textList.begin(); it != m_textList.end(); ++it)
    {
        DamageText *go = (DamageText *)*it;
        if (!go->getActive())
        {
            go->setActive(true);
            return go;
        }
    }
    for (unsigned i = 0; i < 10; ++i)
    {
        DamageText *go = new DamageText();
        m_textList.push_back(go);
    }
    DamageText *go = (DamageText *)m_textList.back();
    go->setActive(true);
    return go;
}

void SceneSP3::RenderTO(DamageText *to)
{  
    to->setScaleText(to->getScaleText() + Vector3(1, 1, 1));
    if (to->getIsEnemy() && !to->getIsHeal() && !to->getIsStamina())
    {
        modelStack.PushMatrix();
        modelStack.Translate(to->getLastHitPos().x, to->getLastHitPos().y, to->getLastHitPos().z);

		float angle;
		Vector3 v = walkCam.GetDir();
		v.y = 0;

		if (!v.IsZero())
		{
			v.Normalize();
			angle = acos(v.Dot(Vector3(0, 0, -1)));
			angle = Math::RadianToDegree(angle);
			if (v.Cross(Vector3(0, 0, -1)).y > 0) angle *= -1.f;
		}
		else angle = 0.f;
		modelStack.Rotate(angle, 0, 1, 0);

		v = walkCam.GetDir();
		if (!v.IsZero())
		{
			v.Normalize();

			angle = asin(v.y);
			angle = Math::RadianToDegree(angle);
		}
		else angle = 0.f;

		modelStack.Rotate(angle, 1, 0, 0);

        modelStack.Scale(to->getScaleText().x, to->getScaleText().y, to->getScaleText().z);
        std::ostringstream ss;
        ss << "-" << to->getLastDamage();
        RenderText(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1));
        modelStack.PopMatrix();
    }
    else if (!to->getIsEnemy() && !to->getIsHeal() && !to->getIsStamina())
    {
        modelStack.PushMatrix();
        modelStack.Translate(to->getLastHitPos().x, to->getLastHitPos().y, to->getLastHitPos().z);
        modelStack.Rotate(LookAtPlayer(walkCam.GetPos(), to->getLastHitPos()), 0, 1, 0);
        modelStack.Scale(to->getScaleText().x, to->getScaleText().y, to->getScaleText().z);
        std::ostringstream ss;
        ss << "-" << to->getLastDamage();
        RenderText(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0));
        modelStack.PopMatrix();
    }
    else if (!to->getIsEnemy() && to->getIsHeal() && !to->getIsStamina())
    {
        modelStack.PushMatrix();
        modelStack.Translate(to->getLastHitPos().x, to->getLastHitPos().y, to->getLastHitPos().z);
        modelStack.Rotate(LookAtPlayer(walkCam.GetPos(), to->getLastHitPos()), 0, 1, 0);
        modelStack.Scale(to->getScaleText().x, to->getScaleText().y, to->getScaleText().z);
        std::ostringstream ss;
        ss << "+" << to->getLastDamage();
        RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0));
        modelStack.PopMatrix();
    }
    else if (!to->getIsEnemy() && !to->getIsHeal() && to->getIsStamina())
    {
        modelStack.PushMatrix();
        modelStack.Translate(to->getLastHitPos().x, to->getLastHitPos().y, to->getLastHitPos().z);
        modelStack.Rotate(LookAtPlayer(walkCam.GetPos(), to->getLastHitPos()), 0, 1, 0);
        modelStack.Scale(to->getScaleText().x, to->getScaleText().y, to->getScaleText().z);
        std::ostringstream ss;
        ss << "+" << to->getLastDamage();
        RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 1, 1));
        modelStack.PopMatrix();
    }

    if (to->getScaleText().x > TextScaleValue)
    {
        to->setScaleText(Vector3(0, 0, 0));
        to->setActive(false);
    }
}

void SceneSP3::UpdateSeaCreatures(double dt)
{
    t_stuckPrevention += dt;
    for (auto it : seaList)
    {
        GameObject* go = (GameObject*)it;
        if (go->active)
        {
            if (go->objectType == GameObject::SEACREATURE)
            {
                SeaCreature* sc = (SeaCreature*)it;
				if (sc->isstunned == false && sc->getisVacuum() == false)
				{
					if (sc->getHealth() <= 0)
					{
						sc->active = false;
						if (sc == skipper->getTarget())
						{
							skipper->setTarget(skipper);
						}
					}

				if (sc->getHealth() <= 0)
				{
					sc->active = false;
					if (sc->seaType == SeaCreature::MINNOW)
						g_MinnowCount--;
					if (sc->seaType == SeaCreature::PUFFER)
						g_PufferfishCount--;
					if (sc->seaType == SeaCreature::FCRAB)
						g_FCrabCount--;
					if (sc->seaType == SeaCreature::CUTTLE)
						g_CuttlefishCount--;
					if (sc->seaType == SeaCreature::CHIMERA)
						g_ChimeraCount--;
					if (sc->seaType == SeaCreature::DRONE)
						g_IsopodDroneCount--;
				}

                if (sc->pos.x < -1000 && t_stuckPrevention > 1.0 || sc->pos.x > 1000 && t_stuckPrevention > 1.0)
                {
                    sc->vel.x *= -1;
                    t_stuckPrevention = 0.0;
                }
                if (sc->pos.y < 0 && t_stuckPrevention > 1.0 || sc->pos.y > 700 && t_stuckPrevention > 1.0)
                {
                    sc->vel.y *= -1;
                    t_stuckPrevention = 0.0;
                }
                if (sc->pos.z < -1000 && t_stuckPrevention > 1.0 || sc->pos.z > 1000 && t_stuckPrevention > 1.0)
                {
                    sc->vel.z *= -1;
                    t_stuckPrevention = 0.0;
                }

                switch (sc->seaType)
                {
                case SeaCreature::MINNOW:
                {
                    Minnow* minnow = (Minnow*)it;

                    if (minnow->getisLeader())
                        minnow->pos += minnow->vel * dt * 3;
                    else if (minnow->state == Minnow::FLOCK)
                        minnow->pos += minnow->vel * dt;
                    else
                        minnow->pos += minnow->vel * dt * 10;

						hitbox2::updatehitbox(minnow->collision, minnow->pos);

						Vector3 tempCentreOfMass(0, 0, 0);
						Vector3 tempRepelVector(0, 0, 0);
						Vector3 tempForceVector(0, 0, 0);
						Vector3 ahead(0, 0, 0);

						if (!minnow->vel.IsZero())
							ahead = minnow->pos + minnow->vel.Normalized() * 5;

                    if (terraincollision(ahead, m_heightMap[SharedData::GetInstance()->SD_CurrentArea]))
                    {
                        minnow->state = Minnow::COLLIDING;
                        minnow->vel *= -1;
                    }
                    else if (minnow->state != Minnow::FLEE)
                    {
                        minnow->state = Minnow::FLOCK;
                    }

                    for (auto it2 : seaList)
                    {
						GameObject* go2 = (GameObject*)it2;
						if (go2->active && minnow != go2)
						{
							if (go2->objectType == GameObject::SEACREATURE)
							{
								SeaCreature* sc = (SeaCreature*)it2;
								if (sc->seaType == SeaCreature::MINNOW)
								{
									Minnow *other = (Minnow *)it2;

									if (other->seaType == SeaCreature::MINNOW && other->getisLeader())
									{
										if (minnow->state == Minnow::FLOCK && !minnow->getisLeader() && (other->pos - minnow->pos).LengthSquared() < g_MinnowAttractRange * 2)
											minnow->vel += minnow->cohesion(other) + minnow->alignment(tempForceVector);
										else if (minnow->state == Minnow::FLEE && !minnow->getisLeader())
											minnow->vel += minnow->seperation(tempCentreOfMass) * 2;
									}
									else if (other->active && minnow != other && minnow->seaType == SeaCreature::MINNOW && minnow->state == Minnow::FLOCK)
									{
										if ((minnow->pos - other->pos).LengthSquared() < g_distFromSeperation)
										{
											tempRepelVector = other->pos - minnow->pos;
											other->vel += other->seperation(tempRepelVector);
										}
									}
								}
							}
						}
                    }

						// Cap velocity
						if (minnow->vel.x > 20)
							minnow->vel.x = 20;
						if (minnow->vel.y > 20)
							minnow->vel.y = 20;
						if (minnow->vel.z > 20)
							minnow->vel.z = 20;
						if (minnow->vel.x < -20)
							minnow->vel.x = -20;
						if (minnow->vel.y < -20)
							minnow->vel.y = -20;
						if (minnow->vel.z < -20)
							minnow->vel.z = -20;

						if (minnow->state == Minnow::FLEE && minnow->getpanicTime() < 3.f)
						{
							minnow->setpanicTime(minnow->getpanicTime() + dt);
						}
						else if (minnow->state == Minnow::FLEE && minnow->getpanicTime() >= 3.f)
						{
							minnow->state = Minnow::FLOCK;
							minnow->setpanicTime(0.f);
						}

                    break;
                }
				case SeaCreature::DRONE:
				{
					Drone *d = (Drone*)it;
					d->m_location = isopod->pos;
					d->UpdateDrone(dt, m_heightMap[4]);
					if (collision(d->m_hitbox, player_box))
					{
						fishVel = -d->vel;
						d->m_state = Drone::RETREAT;
						if (skipper->getTimerReceieveDamage() > 1.0)
						{
							
							skipper->setTimerReceieveDamage(0.0);

							UpdateCapturedPuff(20);

							DamageText* text = FetchTO();
							text->setActive(true);
							text->setLastHitPos(playerpos + walkCam.GetDir().Normalized() * 5 + Vector3(0, 10, 0));
							text->setLastDamage(20);
							text->setScaleText(Vector3(0, 0, 0));
							text->setIsEnemy(false);
						}
					}
					break;
				}
					
				case SeaCreature::PUFFER:
                {
                    Pufferfish* puffer = (Pufferfish*)it;

						hitbox2::updatehitbox(puffer->collision, puffer->pos);
						Vector3 displacement = playerpos - puffer->pos;

                    switch (puffer->pstate)
                    {
                    case Pufferfish::IDLE:
                    {
                        if (collision(puffer->collision, player_box))
                        {
                            puffer->setMoveCoolDown(0.0);
                            puffer->vel *= -1.f;

								if (fishVel.Length() < 3)
									fishVel = Vector3(5, 5, 5);

                            fishVel *= -1.f;
                            walkCam.Move(fishVel * (float)dt);

							UpdateCapturedPuff(10);

							puffer->scale = Vector3(10, 10, 10);
							puffer->pstate = Pufferfish::ENRAGED;
						}
						puffer->pos += puffer->vel * dt;

					break;
					}
					case Pufferfish::ENRAGED:
					{
						if (displacement.LengthSquared() > 400 * 400)
						{
							puffer->vel *= -1.f;
							puffer->scale = Vector3(5, 5, 5);
							puffer->pstate = Pufferfish::IDLE;
						}

						if (collision(puffer->collision, player_box) && skipper->getTimerReceieveDamage() > 1.0)
						{
							skipper->setTimerReceieveDamage(0.0);

                            puffer->setMoveCoolDown(0.0);
                            puffer->vel *= -1.f;

						    if (fishVel.Length() < 3)
							    fishVel = Vector3(5, 5, 5);

                            fishVel *= -1.f;
                            walkCam.Move(fishVel * (float)dt);

						    UpdateCapturedPuff(20);

                        }

							if (puffer->getMoveCoolDown() > 0.5)
							{
								puffer->vel += displacement.Normalized();
							}

							if (puffer->vel.x > 50)
								puffer->vel.x = 50;
							if (puffer->vel.y > 50)
								puffer->vel.y = 50;
							if (puffer->vel.z > 50)
								puffer->vel.z = 50;
							if (puffer->vel.x < -50)
								puffer->vel.x = -50;
							if (puffer->vel.y < -50)
								puffer->vel.y = -50;
							if (puffer->vel.z < -50)
								puffer->vel.z = -50;

							puffer->setMoveCoolDown(puffer->getMoveCoolDown() + dt);
							puffer->pos += puffer->vel * dt;

					    break;
					}
					}

                    // terrain collision
                    if (terraincollision(puffer->collision, m_heightMap[SharedData::GetInstance()->SD_CurrentArea]))
                    {
                        puffer->vel *= -1.f;
                        puffer->pos.y += 4;
                    }

						// height limit
						if (puffer->pos.y > 1000)
						{
							puffer->pos.y -= 1;
							puffer->vel.y = -puffer->vel.y;
						}

						break;
					}
					case SeaCreature::FCRAB:
					{
						Fcrab* crab = (Fcrab*)it;
						
						hitbox2::updatehitbox(crab->collision, crab->pos);

						crab->pos += crab->vel*dt;
						float h = 350.f * ReadHeightMap(m_heightMap[2], crab->pos.x / 3000.f, crab->pos.z / 3000.f) + 4;//get height

						Vector3 displacement = SharedData::GetInstance()->SD_PlayerPos - crab->pos;

						if (crab->pos.y > 300)// reset crabs who move out of range
						{
							float x = Math::RandFloatMinMax(-300, 0);
							float z = Math::RandFloatMinMax(-1000, -800);
							float y = 350.f * ReadHeightMap(m_heightMap[2], x / 3000.f, z / 3000.f) + 4;
							crab->pos.Set(x, y, z);
						}

						switch (crab->FCstate)
						{
						case Fcrab::IDLE:
						{
							float theta = 0;
							if (displacement.LengthSquared() < 50 * 50)
							{
								crab->pos.y += 1;
								theta = Math::RadianToDegree(atan2(displacement.y, Vector3(displacement.x, 0, displacement.z).Length()));
								crab->vel = displacement.Normalized() * 30;
								crab->FCstate = Fcrab::ATTACKING;
							}

							if (crab->pos.y < h)
								crab->pos.y += (float)dt * 10;
							else if (crab->pos.y > h)
								crab->pos.y -= (float)dt * 10;
							break;
						}
						case Fcrab::ATTACKING:
						{
							crab->vel.y -= 9.8f * (float)dt;

							if (collision(crab->aabb, player_box) && skipper->getTimerReceieveDamage() > 1.0)
							{
								skipper->setTimerReceieveDamage(0.0);
                            	fishVel *= -1.f;

								skipper->setHealth(skipper->getHealth() - 20);
								UpdateCapturedPuff(20);

                        }

							if (crab->pos.y <= h)
							{
								crab->vel.Set(Math::RandFloatMinMax(0, 4), 0, Math::RandFloatMinMax(3, 6));

								crab->pos.y = h;
								crab->FCstate = Fcrab::IDLE;
								break;
							}
						}
						}

						crab->UpdateFcrab(dt);//run update for fcrabs

						break;
					}
					case SeaCreature::CUTTLE:
					{
						Cuttlefish* cuttle = (Cuttlefish*)it;

						hitbox2::updatehitbox(cuttle->collision, cuttle->pos);

						if (terraincollision(cuttle->collision, m_heightMap[3]))//collision with terrain
						{
							cuttle->pos.y += 2;
							cuttle->vel = -cuttle->vel;
						}

						if (collision(cuttle->collision, player_box) && skipper->getTimerReceieveDamage() > 1.0)
						{
							skipper->setTimerReceieveDamage(0.0);

                        	fishVel *= -1.f;

							UpdateCapturedPuff(20);

    
                    }

						if (cuttle->ctstate == Cuttlefish::ATTACKING)
						{
							if (cuttle->getIsShoot())
							{
								for (int i = 0; i < 20; ++i)
								{
									Projectile *po = FetchPO();
									po->objectType = GameObject::PROJECTILE;
									po->projectileType = Projectile::INK;
									po->active = true;
									po->scale.Set(0.5f, 0.5f, 0.5f);
									po->pos.Set(cuttle->pos.x + Math::RandFloatMinMax(-3.f, 3.f), cuttle->pos.y + Math::RandFloatMinMax(-3.f, 3.f), cuttle->pos.z + Math::RandFloatMinMax(-3.f, 3.f));
									Vector3 view = (playerpos - cuttle->pos).Normalized();
									po->vel.Set(view.x, view.y, view.z);
									po->setLifetime(3.0);
								}

								cuttle->setIsShoot(false);
							}
						}

						cuttle->UpdateCuttle(dt);

						break;
					}
					case SeaCreature::CHIMERA:
					{
						Chimera* chimera = (Chimera*)it;

						hitbox2::updatehitbox(chimera->collision, chimera->pos);

						if (collision(chimera->aabb, player_box))
							chimera->chstate = Chimera::FLEEING;

						if (collision(chimera->aabb, player_box) && skipper->getTimerReceieveDamage() > 1.0)
						{
							chimera->chstate = Chimera::FLEEING;

							skipper->setTimerReceieveDamage(0.0);

                        	fishVel *= -1.f;

						    UpdateCapturedPuff(20);
                        }
						chimera->UpdateChimera(dt);
						break;
					}
					}
				}
            }
        }
    }
}

void SceneSP3::UpdateProjectile(double dt)
{
	for (auto it : projectileList)
	{
		GameObject* go = (GameObject*)it;
		if (go->active)
		{
			if (go->objectType == GameObject::PROJECTILE)
			{
				Projectile* po = (Projectile*)it;

				po->setLifetime(po->getLifetime() - dt);

				if (po->getLifetime() <= 0.0)
					po->active = false;

				if (po->projectileType == Projectile::SBULLET || po->projectileType == Projectile::PBULLET)
                {
                    po->pos += po->vel * dt;

					if (po->projectileType == Projectile::SBULLET)
						po->pos += po->vel * dt * 50;

                    for (auto it2 : seaList)
                    {
                        GameObject* go2 = (GameObject*)it2;
                        if (go2->active && go != go2 && go2->objectType == GameObject::SEACREATURE)
                        {
                            SeaCreature* sc = (SeaCreature*)it2;

							bool hit = false;

                            if ((po->pos - sc->pos).LengthSquared() < sc->scale.z + sc->scale.z + 50)
                            {
                                po->active = false;
                                sc->setHealth(sc->getHealth() - skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));

                                DamageText* text = FetchTO();
                                text->setActive(true);
                                text->setLastHitPos(po->pos);
                                text->setLastDamage(skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));
                                text->setScaleText(Vector3(0, 0, 0));
                                text->setIsEnemy(true);
                                text->setIsHeal(false);
                                text->setIsStamina(false);
								if (po->projectileType == Projectile::PBULLET)
								{
									skipper->setTarget(sc);
								}

                                hit = true;
                            }

                            switch (sc->seaType)
                            {
                            case SeaCreature::MINNOW:
                            {
                                Minnow* minnow = (Minnow*)it2;
                                if ((minnow->pos - po->pos).LengthSquared() < 5000 && minnow->state != Minnow::FLEE)
                                {
                                    minnow->state = Minnow::FLEE;
                                    minnow->vel.Set(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10));
                                }
                                break;
                            }
                            case SeaCreature::PUFFER:
                            {
                                if (hit)
                                {
                                    Pufferfish* puffer = (Pufferfish*)it2;
                                    puffer->scale = Vector3(10, 10, 10);
                                    if (puffer->pstate == Pufferfish::IDLE)
                                        puffer->pstate = Pufferfish::ENRAGED;
                                }
                                break;
                            }
                            case SeaCreature::FCRAB:
                            {
                                if (hit)
                                {
                                    Fcrab* crab = (Fcrab*)it2;
                                    if (crab->FCstate == Fcrab::IDLE)
                                        crab->FCstate = Fcrab::ATTACKING;
                                }
                                break;
                            }
                            case SeaCreature::CUTTLE:
                            {
                                if (hit)
                                {
                                    Cuttlefish* cuttle = (Cuttlefish*)it2;
                                    if (cuttle->ctstate == Cuttlefish::IDLE)
                                        cuttle->ctstate = Cuttlefish::ATTACKING;
                                }
                                break;
                            }
                            case SeaCreature::CHIMERA:
                            {
                                if (hit)
                                {
                                    Chimera* chimera = (Chimera*)it2;
                                    if (chimera->chstate == Chimera::IDLE)
                                        chimera->chstate = Chimera::ATTACKING;
                                }
                                break;
                            }
                            }
                        }
                        else if (go2->active && go != go2 && go2->objectType == GameObject::BOSS)
                        {
                            Boss* boss = (Boss*)it2;
							if (po->projectileType == Projectile::PBULLET)
							{
								skipper->setTarget(boss);
							}
                            switch (boss->bossType)
                            {
							case Boss::GIANTSQUID:
							{
								GiantSquid *squid = (GiantSquid *)it2;
								// Squid head
								if (collision(squid->collision, po->pos))
								{
									po->active = false;
									squid->setHealth(squid->getHealth() - skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));

									DamageText* text = FetchTO();
									text->setActive(true);
									text->setLastHitPos(po->pos);
									text->setLastDamage(skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));
									text->setScaleText(Vector3(0, 0, 0));
									text->setIsEnemy(true);
									text->setIsHeal(false);
									text->setIsStamina(false);


								}
								// Tentacle
								for (int i = 0; i < 6; ++i)
								{
									if (collision(squid->tentacle[i]->collision, po->pos))
									{
										po->active = false;
										squid->tentacle[i]->setHealth(squid->tentacle[i]->getHealth() - skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));

										DamageText* text = FetchTO();
										text->setActive(true);
										text->setLastHitPos(po->pos);
										text->setLastDamage(skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));
										text->setScaleText(Vector3(0, 0, 0));
										text->setIsEnemy(true);
										text->setIsHeal(false);
										text->setIsStamina(false);

									}
								}
							}
                                break;
							case Boss::GIANTCRAB:
							{
								GiantCrab *crab = (GiantCrab *)it2;
								// Squid head
								if (collision(crab->m_hitbox, po->pos))
								{
									
									if (crab->GetState() == GiantCrab::IDLE)
										crab->SetState(GiantCrab::AGGRO);

									if (crab->GetState() == GiantCrab::GRAB && po->projectileType == Projectile::SBULLET)
										crab->SetState(GiantCrab::STRAFE);

									po->active = false;
									crab->setHealth(crab->getHealth() - skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));
									DamageText* text = FetchTO();
									text->setActive(true);
									text->setLastHitPos(po->pos);
									text->setLastDamage(skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));
									text->setScaleText(Vector3(0, 0, 0));
									text->setIsEnemy(true);
									text->setIsHeal(false);
									text->setIsStamina(false);



									skipper->setHealth(skipper->getHealth() - (skipper->getDamage(), skipper->getBaseDamage())/2 );
									DamageText* text2 = FetchTO();
									text2->setActive(true);
									text2->setLastHitPos(playerpos + walkCam.GetDir().Normalized() * 10 + Vector3(0, 10, 0));
									text2->setLastDamage((skipper->getDamage(), skipper->getBaseDamage())/2);
									text2->setScaleText(Vector3(0, 0, 0));
									text2->setIsEnemy(false);
									text2->setIsHeal(false);
									text2->setIsStamina(false);

								}
							}
							break;

							case Boss::FRILLEDSHARK:
							{
								FrilledShark *fshark = (FrilledShark *)it2;
								// Squid head
								for (unsigned i = 0; i < 5; ++i)
								{
									if (collision(fshark->m_FSbox[i], po->pos))
									{

										if (fshark->m_state == FrilledShark::IDLE)
											fshark->m_state = FrilledShark::AGGRO;

										po->active = false;
										fshark->setHealth(fshark->getHealth() - skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));
										DamageText* text = FetchTO();
										text->setActive(true);
										text->setLastHitPos(po->pos);
										text->setLastDamage(skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));
										text->setScaleText(Vector3(0, 0, 0));
										text->setIsHeal(false);
										text->setIsStamina(false);
										text->setIsEnemy(true);

									}
								}
							}
							break;
							case Boss::ISOPOD:
							{
								Isopod *ipod = (Isopod *)it2;
								// Squid head

								if (collision(ipod->m_hitbox, po->pos))
								{
									if (ipod->m_state == Isopod::IDLE)
										ipod->m_state = Isopod::AGGRO;

									po->active = false;
									ipod->setHealth(ipod->getHealth() - skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));
									DamageText* text = FetchTO();
									text->setActive(true);
									text->setLastHitPos(po->pos);
									text->setLastDamage(skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));
									text->setScaleText(Vector3(0, 0, 0));
									text->setIsHeal(false);
									text->setIsStamina(false);
									text->setIsEnemy(true);

								}

							}
							break;
							}
						}
					}
				}
				else if (po->projectileType == Projectile::INK)
				{
					po->pos += po->vel * dt * 500;
					if ((po->pos - playerpos).LengthSquared() < (po->scale.x * po->scale.x) + (skipper->scale.x * skipper->scale.x))
					{
						po->active = false;

						skipper->setHealth(skipper->getHealth() - 20);
						UpdateCapturedPuff(20);

                    }
                }
            }
        }
    }
}
void SceneSP3::UpdateCapturedPuff(int hp)
{
	bool temp = false;
	for (std::vector<GameObject *>::iterator it = SharedData::GetInstance()->SD_CapturedList.begin(); it != SharedData::GetInstance()->SD_CapturedList.end(); ++it)
	{
		SeaCreature *go = (SeaCreature *)*it;
		if (go->active)
		{
			if (go->objectType == GameObject::CAPTURED && go->seaType == SeaCreature::PUFFER)
			{
				go->setHealth(go->getHealth() - hp);
				temp = true;
				DamageText* text = FetchTO();
				text->setActive(true);
				text->setLastHitPos(go->pos + Vector3(0, 10, 0));
				text->setLastDamage(hp);
				text->setScaleText(Vector3(0, 0, 0));
				text->setIsEnemy(false);
				text->setIsHeal(false);
				text->setIsStamina(false);
				break;
			}
		}
	}
	if (temp == false)
	{
		skipper->setHealth(skipper->getHealth() - hp);
		
		DamageText* text = FetchTO();
		text->setActive(true);
		text->setLastHitPos(playerpos + walkCam.GetDir().Normalized() * 5 + Vector3(0, 10, 0));
		text->setLastDamage(hp);
		text->setScaleText(Vector3(0, 0, 0));
		text->setIsEnemy(false);
		text->setIsHeal(false);
		text->setIsStamina(false);

	}

}
void SceneSP3::UpdateCaptured(double dt)
{
	// Minnow loop
	for (std::vector<GameObject *>::iterator it = SharedData::GetInstance()->SD_CapturedList.begin(); it != SharedData::GetInstance()->SD_CapturedList.end(); ++it)
	{

		SeaCreature *fo = (SeaCreature *)*it;
        if (fo->getHealth() <= 0 && fo->active && fo->objectType == GameObject::CAPTURED)
		{
            switch (fo->seaType)
            {
            case SeaCreature::MINNOW:
                g_countCapturedMinnow--;
            case SeaCreature::PUFFER:
                g_countCapturedPuffer--;
            case SeaCreature::FCRAB:
                g_countCapturedCrab--;
            case SeaCreature::CUTTLE:
                g_countCapturedCuttle--;
            case SeaCreature::CHIMERA:
                g_countCapturedChimera--;
            }

			fo->active = false;
		}
		if (fo->active)
		{
			if (fo->objectType == GameObject::CAPTURED)
			{			
					fo->pos += fo->vel * dt;
					// Collision
					if (fo->seaType == SeaCreature::CHIMERA || fo->seaType == SeaCreature::FCRAB)
					hitbox::updatehitbox(fo->aabb, fo->pos);

					else
					hitbox2::updatehitbox(fo->collision, fo->pos);
					

					Vector3 tempCentreOfMass(0, 0, 0);
					Vector3 tempRepelVector(0, 0, 0);
					Vector3 tempForceVector(0, 0, 0);

					if (terraincollision(fo->collision, m_heightMap[SharedData::GetInstance()->SD_CurrentArea]))
					{
						fo->vel *= -5;
					}

                    for (std::vector<GameObject *>::iterator it2 = captureList.begin(); it2 != captureList.end(); ++it2)
					{
						Minnow *other = (Minnow *)*it2;
						if (other->active)
						{
							if ((playerpos - other->pos).LengthSquared() < g_distFromSeperation && (other->seaType == SeaCreature::MINNOW || other->seaType == SeaCreature::CUTTLE || other->seaType == SeaCreature::PUFFER || skipper->getTarget()->objectType == GameObject::PLAYER))
							{
								tempRepelVector = other->pos - playerpos;
								other->vel += other->seperation(tempRepelVector);
							}
						}
						if ((fo->pos - other->pos).LengthSquared() < g_distFromSeperation)
						{
							tempRepelVector = other->pos - fo->pos;
							other->vel += other->seperation(tempRepelVector);
						}
					}
					if (fo->seaType==SeaCreature::MINNOW || fo->seaType==SeaCreature::CUTTLE || fo->seaType==SeaCreature::PUFFER || skipper->getTarget()->objectType == GameObject::PLAYER)
					{
						fo->vel += fo->cohesion(playerpos, walkCam.GetDir()) + fo->alignment(tempForceVector);

					}
					
					// Cap velocity
					if (fo->vel.x > 20)
						fo->vel.x = 20;
					if (fo->vel.y > 20)
						fo->vel.y = 20;
					if (fo->vel.z > 20)
						fo->vel.z = 20;
					if (fo->vel.x < -20)
						fo->vel.x = -20;
					if (fo->vel.y < -20)
						fo->vel.y = -20;
					if (fo->vel.z < -20)
						fo->vel.z = -20;
					if ((playerpos-fo->pos).LengthSquared() > 100000)
					{
						fo->pos.Set(Math::RandFloatMinMax(playerpos.x + 20, playerpos.x - 20), Math::RandFloatMinMax(playerpos.y + 20, playerpos.y - 20), Math::RandFloatMinMax(playerpos.z + 20, playerpos.z-20));
					}

				
			}
		}
	}
}

void SceneSP3::UpdateTravel()
{
	if ( collision(m_travelzonedown, player_box.m_point[0]) || collision(m_travelzonedown, player_box.m_point[6]))//travel downward
	{

		if (static_cast<SharedData::AREA>(SharedData::GetInstance()->SD_CurrentArea) != SharedData::A_NIGHTMARETRENCH)
		{
            SharedData::GetInstance()->SD_PlayerHealth = skipper->getHealth();
            SharedData::GetInstance()->SD_SceneLoaded = false;
			SharedData::GetInstance()->SD_Down = true;
			SharedData::GetInstance()->SD_CurrentArea += 1;
            Application::sceneManager->SetPreviousScene(Application::sceneManager->GetCurrentScene());
            Application::sceneManager->SetCurrentScene(Application::sceneManager->GetLoadingScreen());
			std::cout << "down" << std::endl;
            return;
		}
	}
	if (collision(m_travelzoneup, player_box.m_point[0]) || collision(m_travelzoneup, player_box.m_point[6]))//travel upward
	{
		if (static_cast<SharedData::AREA>(SharedData::GetInstance()->SD_CurrentArea) != SharedData::A_TUTORIAL)
		{
            SharedData::GetInstance()->SD_PlayerHealth = skipper->getHealth();
            SharedData::GetInstance()->SD_SceneLoaded = false;
			SharedData::GetInstance()->SD_Down = false;
			SharedData::GetInstance()->SD_CurrentArea -= 1;
            Application::sceneManager->SetPreviousScene(Application::sceneManager->GetCurrentScene());
            Application::sceneManager->SetCurrentScene(Application::sceneManager->GetLoadingScreen());
			
			std::cout << "up" << std::endl;
            return;
		}
	}

}

void SceneSP3::Update(double dt)
{
	fps = 1.0f / (float)dt;
    const float acceleration = 50.f;
    const float speedLimit = 50.f;
	vrotate += dt*100;

	UpdateKeys();
	UpdatePauseFunction();
    UpdatePauseScreen(dt);
    UpdateDeathScreen(dt);

    if (GetKeyState(VK_NUMPAD9, KEY_STATUS_DOWN))
        SharedData::GetInstance()->SD_TurnFPSOn = !SharedData::GetInstance()->SD_TurnFPSOn;

    if (GetKeyState(VK_NUMPAD1, KEY_STATUS_DOWN))
        giantSquid->setHealth(0);
    if (GetKeyState(VK_NUMPAD2, KEY_STATUS_DOWN))
        giantCrab->setHealth(0);
    //if (GetKeyState(VK_NUMPAD3, KEY_STATUS_DOWN))
        //frilledshark->setHealth(0);
    if (GetKeyState(VK_NUMPAD4, KEY_STATUS_DOWN))
        isopod->setHealth(0);

    if (GetKeyState('C', KEY_STATUS_DOWN))
    {
        std::cout << playerpos << std::endl;
    }

	if (isGamePaused) return;


    if (Application::IsMousePressed(0) && m_fireRate > 0.25f)
    {
        Application::ShootSound = Application::SoundEngine->play2D("audio//shoot.wav", false, true);

        Projectile *po = FetchPO();

        static const float focusLength = 300.f;

        static const float projectileSpeed = 600.f;

        Vector3 focusPoint = walkCam.GetPos() + Vector3(0, walkCam.yOffset, 0) + walkCam.GetDir() * (focusLength - walkCam.distance);

        po->objectType = GameObject::PROJECTILE;
        po->projectileType = Projectile::PBULLET;
        po->active = true;
        po->scale.Set(10, 10, 10);
        po->pos = playerpos;

        po->setLifetime(3.0);
        Vector3 bulletDirection = focusPoint - playerpos;
        bulletDirection.Normalize();
        po->vel = bulletDirection * projectileSpeed + fishVel;
        m_fireRate = 0.f;
    }
    
	m_fireRate += dt;
    Vector3 temp = walkCam.GetPos();
    Vector3 right = walkCam.GetDir().Cross(walkCam.GetUp());
    right.Normalize();
    float movespeed = 1000;
    if (GetKeyState('Z', KEY_STATUS_DOWN))
    {
        val += 20.f * (float)dt;
        cout << val << endl;
    }
    if (GetKeyState('X', KEY_STATUS_DOWN))
    {
        val -= 20.f * (float)dt;
        cout << val << endl;
    }

    skipper->setTimerReceieveDamage(skipper->getTimerReceieveDamage() + dt);
    
    static float staminaFactor = 1.f;

    Vector3 forceApplied;
    forceApplied.SetZero();

    if (!SharedData::GetInstance()->SD_IsImmobile)
    {
        if (GetKeyState('W'))
        {
            forceApplied += walkCam.GetDir() * acceleration;
        }
        if (GetKeyState('S'))
        {
            forceApplied -= walkCam.GetDir() * acceleration;
        }

        if (GetKeyState('A'))
        {
            forceApplied -= right * acceleration;
        }
        if (GetKeyState('D'))
        {
            forceApplied += right * acceleration;
        }

        for (unsigned char i = 0; i < 256u; ++i)
        {
            break; //Comment this break break statement to test keys.
            if (GetKeyState(i)) std::cout << i << std::endl;
        }

        //Correspondance:
        //Is held; Is pressed
        static bool boostKeyStatus[] = { false, false, false };

        boostKeyStatus[0] = GetKeyState(VK_SHIFT);
        boostKeyStatus[2] = false;

        if (boostKeyStatus[0] && !boostKeyStatus[1])
        {
            boostKeyStatus[2] = true;
            boostKeyStatus[1] = true;
        }

        if (!boostKeyStatus[0] && boostKeyStatus[1])
        {
            boostKeyStatus[1] = false;
        }

        switch (skipper->boostStatus)
        {
        case Skipper::BOOST_READY:
            if (boostKeyStatus[2])
                skipper->boostStatus = Skipper::BOOST_ACTIVE;
            break;
        case Skipper::BOOST_ACTIVE:
            staminaFactor = min(staminaFactor + (float)dt * 1.5f, 3);
            skipper->stamina -= (float)dt * 25.f;
            if (skipper->stamina <= 0.f)
            {
                skipper->stamina = 0.f;
                skipper->boostStatus = Skipper::BOOST_RECHARGING;
            }
            break;
        case Skipper::BOOST_RECHARGING:
            staminaFactor = max(staminaFactor - (float)dt * 0.75f, 1);
            skipper->stamina += (float)dt * 15.f;
            if (skipper->stamina >= 100.f)
            {
                skipper->stamina = 100.f;
                skipper->boostStatus = Skipper::BOOST_READY;
            }
            break;
        }

        fishVel +=
            forceApplied
            * (skipper->boostStatus == Skipper::BOOST_ACTIVE ? 2.f : 1.f)
            * (float)dt;

        if (fishVel.LengthSquared() > speedLimit * speedLimit * staminaFactor * staminaFactor)
        {
			fishVel.Normalize();
			fishVel *= speedLimit * staminaFactor;
        }

        walkCam.Move(fishVel * (float)dt);

        playerpos = walkCam.GetPos() + Vector3(0, 80, 0);
        SharedData::GetInstance()->SD_PlayerPos = playerpos;
        hitbox2::updatehitbox(player_box, playerpos);



        if (terraincollision(player_box, m_heightMap[SharedData::GetInstance()->SD_CurrentArea]))
        {
            fishVel *= -1.f;
            walkCam.Move(fishVel * (float)dt);
            playerpos = walkCam.GetPos() + Vector3(0, 80, 0);
            hitbox2::updatehitbox(player_box, playerpos);
        }

        if (!fishVel.IsZero())
        {
            const float frictionValue = 10.f;
            float f = frictionValue * (float)dt;

            if (fishVel.LengthSquared() < f * f)
            {
                fishVel.SetZero();
            }
            else
            {
                fishVel -= fishVel.Normalized() * f;
            }
        }

        if (fishVel != 0)//animate tail
        {
            if (!fish_tailmax)
            {
                if (fish_tailrot < 30)
                    fish_tailrot += fishVel.Length()*(float)dt * 10;
                else
                    fish_tailmax = true;
            }
            else
            {
                if (fish_tailrot > -30)
                    fish_tailrot -= fishVel.Length()*(float)dt*10.f;
                else
                    fish_tailmax = false;
            }
        }

        if (Application::camera_yaw != 0)
        {
            walkCam.Turn(Math::RadianToDegree(-(float)Application::camera_yaw * 0.5f));
        }

        if (Application::camera_pitch != 0)
        {
            walkCam.Pitch(Math::RadianToDegree(-(float)Application::camera_pitch * 0.5f));
        }

        Vector3 camDir = walkCam.GetDir();
        if (camDir.x != 0 || camDir.z != 0)
            fishRot.y = Math::RadianToDegree(atan2(-camDir.z, camDir.x));

        fishRot.x = 90 - Math::RadianToDegree(acos(-camDir.y));

        if (skipper->getHealth() <= 0)
        {
            skipper->setIsDead(true);
            SharedData::GetInstance()->SD_IsImmobile = true;
        }

        if (GetKeyState(VK_NUMPAD8, KEY_STATUS_DOWN))
            skipper->setHealth(0);
    }

	for (std::vector<GameObject *>::iterator it = seaList.begin(); it != seaList.end(); ++it)
	{
		SeaCreature *go = (SeaCreature *)*it;
		if (go->active)
		{
			if (go->objectType == GameObject::SEACREATURE)
			{
				go->setisVacuum(false);

			}
		}
	}

	if (Application::IsMousePressed(1))
	{
        for (std::vector<GameObject *>::iterator it = seaList.begin(); it != seaList.end(); ++it)
		{
			SeaCreature *go = (SeaCreature *)*it;
			if (go->active)
			{
				if (go->objectType == GameObject::SEACREATURE)
				{
					
					go->setisVacuum(Capture::rangeCheckXZ(walkCam, *go, playerpos) && go->getHealth() <= 50);
					go->pos.Set(Capture::Vacuum(*go, playerpos, Capture::rangeCheckXZ(walkCam, *go, playerpos)).x,
					            Capture::Vacuum(*go, playerpos, Capture::rangeCheckXZ(walkCam, *go, playerpos)).y,
					            Capture::Vacuum(*go, playerpos, Capture::rangeCheckXZ(walkCam, *go, playerpos)).z);
					go->objectType = Capture::AddSquad(*go, playerpos, Capture::rangeCheckXZ(walkCam, *go, playerpos));
                    if (go->objectType == GameObject::CAPTURED)
					{
						if (skipper->getTarget() == go)
						{
							skipper->setTarget(skipper);
						}
						SharedData::GetInstance()->SD_CapturedList.push_back(go);

                        switch (go->seaType)
                        {
                        case SeaCreature::MINNOW:
                            g_countCapturedMinnow++;
                        case SeaCreature::PUFFER:
                            g_countCapturedPuffer++;
                        case SeaCreature::FCRAB:
                            g_countCapturedCrab++;
                        case SeaCreature::CUTTLE:
                            g_countCapturedCuttle++;
                        case SeaCreature::CHIMERA:
                            g_countCapturedChimera++;
                        }

						int index = std::distance(seaList.begin(), it);
						seaList.erase(seaList.begin()+index);
						break;
					}
				}
			}
		}
		for (std::vector<GameObject *>::iterator it = coralList.begin(); it != coralList.end(); ++it)
		{
			Coral *go = (Coral *)*it;
			if (go->active)
			{
				if (go->objectType == GameObject::CORAL)
				{

					float distance = fabs(playerpos.x - go->pos.x) + fabs(playerpos.y - go->pos.y) + fabs(playerpos.z - go->pos.z);
					Vector3 view = (go->pos - playerpos).Normalized();
					if (fabs(go->pos.x - playerpos.x) >= 1 && distance >= 1)
					{
						go->pos.Set((go->pos.x - view.x / dt / distance), (go->pos.y), (go->pos.z));
					}
					if (fabs(go->pos.y - playerpos.y) >= 1 && distance >= 1)
					{
						go->pos.Set(go->pos.x, (go->pos.y - view.y / dt / distance), (go->pos.z));
					}
					if (fabs(go->pos.z - playerpos.z) >= 1 && distance >= 1)
					{
						go->pos.Set(go->pos.x, (go->pos.y), (go->pos.z - view.z / dt / distance));
					}

					if ((playerpos - go->pos).LengthSquared() <= 10)
					{
						go->active = false;
						if (go->coralType == Coral::HEALTH)
							skipper->setHealthPackCount(skipper->getHealthPackCount() + 1);
						else if (go->coralType == Coral::STAMINA)
							skipper->setStaminaPackCount(skipper->getStaminaPackCount() + 1);
					}
				}
			}
		}
		}	

        
	UpdateParticles(dt);
	UpdateTravel();
	UpdateSeaCreatures(dt);
	UpdateProjectile(dt);
	UpdateCaptured(dt);
	UpdateSquadFire(dt);
	UpdateSpawner(dt);
	UpdateStunned(dt);
	

    if (GetKeyState('1', KEY_STATUS_DOWN))
    {
        if (!skipper->getIsDead() && skipper->getHealthPackCount() > 0 && skipper->getHealth() < g_MaxSkipperHealth)
        {
            skipper->setHealthPackCount(skipper->getHealthPackCount() - 1);
            skipper->setHealth(skipper->getHealth() + 100);

            DamageText* text = FetchTO();
            text->setActive(true);
            text->setLastHitPos(playerpos + fishVel + Vector3(0, 3.f, 0));
            text->setLastDamage(100);
            text->setScaleText(Vector3(0, 0, 0));
            text->setIsEnemy(false);
            text->setIsHeal(true);
            text->setIsStamina(false);
        }
    }

    if (GetKeyState('2', KEY_STATUS_DOWN))
    {
        if (!skipper->getIsDead() && skipper->getStaminaPackCount() > 0 && skipper->stamina < 100)
        {
            skipper->setStaminaPackCount(skipper->getStaminaPackCount() - 1);
            skipper->stamina += 50;
            if (skipper->stamina > 100)
                skipper->stamina = 100;

            DamageText* text = FetchTO();
            text->setActive(true);
            text->setLastHitPos(playerpos + fishVel + Vector3(0, 3.f, 0));
            text->setLastDamage(100);
            text->setScaleText(Vector3(0, 0, 0));
            text->setIsEnemy(false);
            text->setIsHeal(false);
            text->setIsStamina(true);
        }
    }

    if (GetKeyState('Q', KEY_STATUS_DOWN))
    {
        if (!SharedData::GetInstance()->SD_ObjectiveTabOn)
            SharedData::GetInstance()->SD_ObjectiveTabOn = true;
        else if (SharedData::GetInstance()->SD_ObjectiveTabOn)
            SharedData::GetInstance()->SD_ObjectiveTabOn = false;
    }

	if (GetKeyState('N', KEY_STATUS_DOWN))
	{
		skipper->setTarget(skipper);
	}

    if (SharedData::GetInstance()->SD_ToMenu)
    {
        Application::sceneManager->SetPreviousScene(Application::sceneManager->GetCurrentScene());
        Application::sceneManager->SetMenuScene(new SceneMenu());
        Application::sceneManager->GetMenuScreen()->Init();
        Application::sceneManager->SetCurrentScene(Application::sceneManager->GetMenuScreen());
        SharedData::GetInstance()->SD_ToMenu = false;
    }
}

void SceneSP3::UpdateKeys()
{
	for (unsigned short i = 0; i < (unsigned short)255; ++i)
	{
		keyStates[KEY_STATUS_CUR][i] = Application::IsKeyPressed(i);
		keyStates[KEY_STATUS_DOWN][i] = keyStates[KEY_STATUS_CUR][i] && !keyStates[KEY_STATUS_TEMP][i];
		keyStates[KEY_STATUS_UP][i] = !keyStates[KEY_STATUS_CUR][i] && keyStates[KEY_STATUS_TEMP][i];
		keyStates[KEY_STATUS_TEMP][i] = keyStates[KEY_STATUS_CUR][i];

		continue; //Comment out this line to test keys.
		if (keyStates[KEY_STATUS_DOWN][i]) std::cout << i << std::endl;
	}
}

void SceneSP3::UpdatePauseFunction()
{
    if (GetKeyState(VK_ESCAPE, KEY_STATUS_DOWN) && !skipper->getIsDead() && SharedData::GetInstance()->SD_ContinueInstruction1 && SharedData::GetInstance()->SD_ContinueInstruction2)
    {
        SharedData::GetInstance()->SD_PauseMenu = true;
		pauseChoice = 0;
	}
}

void SceneSP3::UpdatePauseScreen(double dt)
{
    if (SharedData::GetInstance()->SD_PauseMenu && SharedData::GetInstance()->SD_ContinueInstruction1 && SharedData::GetInstance()->SD_ContinueInstruction2)
    {
        isGamePaused = true;

	    if (GetKeyState('S', KEY_STATUS_DOWN))
		    ++pauseChoice;
	    if (GetKeyState(VK_DOWN, KEY_STATUS_DOWN))
		    ++pauseChoice;
	    if (GetKeyState('W', KEY_STATUS_DOWN))
		    --pauseChoice;
	    if (GetKeyState(VK_UP, KEY_STATUS_DOWN))
		    --pauseChoice;

		if (GetKeyState(VK_RETURN, KEY_STATUS_DOWN))
		{
			switch (pauseChoice)
			{
			case 0:
				SharedData::GetInstance()->SD_PauseMenu = false;
				isGamePaused = false;
				break;
			case 1:
				Application::sceneManager->SetPreviousScene(Application::sceneManager->GetCurrentScene());
				Application::sceneManager->SetMenuScene(new SceneMenu());
				Application::sceneManager->GetMenuScreen()->Init();
				Application::sceneManager->SetCurrentScene(Application::sceneManager->GetMenuScreen());
				isGamePaused = false;
				break;
			case 2:
				SharedData::GetInstance()->SD_QuitGame = true;
				isGamePaused = false;
				break;
			}
		}

        pauseChoice = min(max(pauseChoice, 0), 2);
    }
}

void SceneSP3::UpdateSpawner(double dt)
{
    // Spawner
    CoralSpawner.CheckCount(g_CoralHealthCount, g_MaxCoralHealth);

    if (CoralSpawner.getIsSpawn())
    {
        Vector3 tv(Math::RandFloatMinMax(-1000.f, 1000.f), 0, Math::RandFloatMinMax(-1000.f, 1000.f));
        if (terraincollision(tv, m_heightMap[SharedData::GetInstance()->SD_CurrentArea]))
        {
            Coral *c = FetchCoral();
            c->active = true;
            c->objectType = GameObject::CORAL;
            c->coralType = Coral::HEALTH;
            c->scale.Set(5, 5, 5);
            float y = 350.f * ReadHeightMap(m_heightMap[SharedData::GetInstance()->SD_CurrentArea], tv.x / 3000.f, tv.z / 3000.f) + 4;
            c->pos.Set(tv.x, y, tv.z);
            c->vel.Set(0, 0, 0);
            g_CoralHealthCount++;
        }
    }

    CoralSpawner.CheckCount(g_CoralStaminaCount, g_MaxCoralStamina);

    if (CoralSpawner.getIsSpawn())
    {
        Vector3 tv(Math::RandFloatMinMax(-1000.f, 1000.f), 0, Math::RandFloatMinMax(-1000.f, 1000.f));
        if (terraincollision(tv, m_heightMap[SharedData::GetInstance()->SD_CurrentArea]))
        {
            Coral *c = FetchCoral();
            c->active = true;
            c->objectType = GameObject::CORAL;
            c->coralType = Coral::STAMINA;
            c->scale.Set(5, 5, 5);
            float y = 350.f * ReadHeightMap(m_heightMap[SharedData::GetInstance()->SD_CurrentArea], tv.x / 3000.f, tv.z / 3000.f) + 4;
            c->pos.Set(tv.x, y, tv.z);
            c->vel.Set(0, 0, 0);
            g_CoralStaminaCount++;
        }
    }

    MinnowLeaderSpawner.CheckCount(g_MinnowLeaderCount, g_MaxMinnowLeader);

    if (MinnowLeaderSpawner.getIsSpawn())
    {
        Vector3 tv1(Math::RandFloatMinMax(-1000.f, 1000.f), Math::RandFloatMinMax(200.f, 600.f), Math::RandFloatMinMax(-1000.f, 1000.f));
        if (!terraincollision(tv1, m_heightMap[SharedData::GetInstance()->SD_CurrentArea]))
        {
            Minnow* minnowLeader = FetchMinnow();
            minnowLeader->active = true;
            minnowLeader->objectType = GameObject::SEACREATURE;
            minnowLeader->seaType = SeaCreature::MINNOW;
            minnowLeader->state = Minnow::FLOCK;
            minnowLeader->setHealth(50);
            minnowLeader->setisLeader(true);
            minnowLeader->scale.Set(1, 1, 1);
            minnowLeader->pos.Set(tv1.x, tv1.y, tv1.z);
            minnowLeader->vel.Set(Math::RandFloatMinMax(-20, 20), Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-20, 20));
            minnowLeader->collision = hitbox2::generatehitbox(minnowLeader->pos, 10, 10, 10);
            g_MinnowLeaderCount++;

            for (int i = 0; i <= 10; ++i)
            {
                Minnow* minnow = FetchMinnow();
                minnow->active = true;
                minnow->objectType = GameObject::SEACREATURE;
                minnow->seaType = SeaCreature::MINNOW;
                minnow->state = Minnow::FLOCK;
                minnow->setHealth(50);
                minnow->setisLeader(false);
                minnow->scale.Set(1, 1, 1);
                minnow->pos.Set(tv1.x + Math::RandFloatMinMax(-20, 20), tv1.y + Math::RandFloatMinMax(-20, 20), tv1.z + Math::RandFloatMinMax(-20, 20));
                minnow->vel.Set(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10));
                minnow->collision = hitbox2::generatehitbox(minnow->pos, 10, 10, 10);
                g_MinnowCount++;
            }
        }
    }

    MinnowSpawner.CheckCount(g_MinnowCount, g_MaxMinnow);

    if (MinnowSpawner.getIsSpawn())
    {
        Vector3 tv2(Math::RandFloatMinMax(-1000.f, 1000.f), Math::RandFloatMinMax(200.f, 600.f), Math::RandFloatMinMax(-1000.f, 1000.f));
        if (!terraincollision(tv2, m_heightMap[SharedData::GetInstance()->SD_CurrentArea]))
        {
            Minnow* minnow = FetchMinnow();
            minnow->active = true;
            minnow->objectType = GameObject::SEACREATURE;
            minnow->seaType = SeaCreature::MINNOW;
            minnow->state = Minnow::FLOCK;
            minnow->setHealth(50);
            minnow->setisLeader(false);
            minnow->scale.Set(1, 1, 1);
            minnow->pos.Set(tv2.x, tv2.y, tv2.z);
            minnow->vel.Set(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10));
            minnow->collision = hitbox2::generatehitbox(minnow->pos, 10, 10, 10);
            g_MinnowCount++;
        }
    }

    switch (SharedData::GetInstance()->SD_CurrentArea)
    {
    case 1:
    {
        PufferfishSpawner.CheckCount(g_PufferfishCount, g_MaxPufferfish);

        if (PufferfishSpawner.getIsSpawn())
        {
            Vector3 tv(Math::RandFloatMinMax(-1000.f, 1000.f), Math::RandFloatMinMax(200.f, 600.f), Math::RandFloatMinMax(-1000.f, 1000.f));
            if (!terraincollision(tv, m_heightMap[SharedData::GetInstance()->SD_CurrentArea]))
            {
                Pufferfish* pf = FetchPuffer();
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
            }
        }
        break;
    }
    case 2:
    {
        FCrabSpawner.CheckCount(g_FCrabCount, g_MaxFCrab);

        if (FCrabSpawner.getIsSpawn())
        {
            Vector3 tv(Math::RandFloatMinMax(-1000.f, 1000.f), Math::RandFloatMinMax(200.f, 600.f), Math::RandFloatMinMax(-1000.f, 1000.f));
            if (!terraincollision(tv, m_heightMap[SharedData::GetInstance()->SD_CurrentArea]))
            {
                Fcrab *c = FetchFCrab();
                c->active = true;
                c->objectType = GameObject::SEACREATURE;
                c->seaType = SeaCreature::FCRAB;
                c->scale.Set(5, 5, 5);
                c->pos.Set(tv.x, tv.y, tv.z);
                c->vel.Set(Math::RandFloatMinMax(0, 4), 0, Math::RandFloatMinMax(3, 6));
                c->FCstate = Fcrab::IDLE;
                c->aabb = hitbox::generatehitbox(c->pos, 8, 8, 8, NULL);
                c->setHealth(200);
                g_FCrabCount++;
            }
        }
        break;
    }
    case 3:
    {
        CuttlefishSpawner.CheckCount(g_CuttlefishCount, g_MaxCuttlefish);

        if (CuttlefishSpawner.getIsSpawn())
        {
            Vector3 tv(Math::RandFloatMinMax(-1000.f, 1000.f), Math::RandFloatMinMax(200.f, 600.f), Math::RandFloatMinMax(-1000.f, 1000.f));
            if (!terraincollision(tv, m_heightMap[SharedData::GetInstance()->SD_CurrentArea]))
            {
                Cuttlefish* cf = FetchCuttle();
                cf->active = true;
                cf->objectType = GameObject::SEACREATURE;
                cf->seaType = SeaCreature::CUTTLE;
                cf->ctstate = Cuttlefish::IDLE;
                cf->scale.Set(4, 4, 4);
                cf->pos.Set(tv.x, tv.y, tv.z);
                cf->vel.Set(Math::RandFloatMinMax(-40, 40), Math::RandFloatMinMax(-20, 20), Math::RandFloatMinMax(-40, 40));
                cf->collision = hitbox2::generatehitbox(cf->pos, 5, 5, 5);
                cf->setHealth(200);
                cf->setIsShoot(false);
                cf->setShootBufferTime(0.0);
                g_CuttlefishCount++;
            }
        }
        break;
    }
    case 4:
    {
        ChimeraSpawner.CheckCount(g_ChimeraCount, g_MaxChimera);

        if (ChimeraSpawner.getIsSpawn())
        {
            Vector3 tv(Math::RandFloatMinMax(-1000.f, 1000.f), Math::RandFloatMinMax(200.f, 600.f), Math::RandFloatMinMax(-1000.f, 1000.f));
            if (!terraincollision(tv, m_heightMap[SharedData::GetInstance()->SD_CurrentArea]))
            {
                Chimera*c = FetchChimera();
                c->active = true;
                c->objectType = GameObject::SEACREATURE;
                c->seaType = SeaCreature::CHIMERA;
                c->chstate = Chimera::IDLE;
                c->scale.Set(20, 20, 20);
                c->pos.Set(tv.x, tv.y, tv.z);
                c->vel.Set(0, Math::RandFloatMinMax(-20, 20), 0);
                c->aabb = hitbox::generatehitbox(c->pos, 10, 10, 10, NULL);
                c->setHealth(200);
                g_ChimeraCount++;
            }
        }

		if (SharedData::GetInstance()->SD_SceneLoaded)
		{
			if (isopod->getSpawnIsopodDrone())
			{
				IsopodDroneSpawner.CheckCount(g_IsopodDroneCount, g_MaxIsopodDrone);

				if (IsopodDroneSpawner.getIsSpawn())
				{
					Drone *d = FetchDrone();
					d->active = true;
					d->objectType = GameObject::SEACREATURE;
					d->seaType = SeaCreature::DRONE;
					d->scale.Set(10, 10, 10);
					d->pos.Set(isopod->pos.x, isopod->pos.y+20, isopod->pos.z);
					d->vel  = Vector3(Math::RandFloatMinMax(-50, 50), Math::RandFloatMinMax(-20, 20), Math::RandFloatMinMax(-50, 50));
					d->setHealth(400);
					g_IsopodDroneCount++;
				}
			}
		}

        break;
    }
    }
}

void SceneSP3::UpdateSquadFire(double dt)
{
    for (std::vector<GameObject *>::iterator it = SharedData::GetInstance()->SD_CapturedList.begin(); it != SharedData::GetInstance()->SD_CapturedList.end(); ++it)
    {
        GameObject *go = (GameObject *)*it;
        if (go->active)
        {
            if (go->objectType == GameObject::CAPTURED)
            {
                SeaCreature *other = (SeaCreature *)*it;
                if (skipper->getTarget()->objectType != GameObject::PLAYER && skipper->getTarget()->active == true)
                {

                    if (other->getDebounceTimer() > 3 && (other->seaType == SeaCreature::MINNOW || other->seaType == SeaCreature::CUTTLE))
                    {
                        for (std::vector<GameObject *>::iterator it2 = seaList.begin(); it2 != seaList.end(); ++it2)
                        {
                            GameObject *go2 = (GameObject *)*it2;
                            if (go2 == skipper->getTarget())
                            {
                                Projectile *po = FetchPO();
                                po->setLifetime(10);
                                po->objectType = GameObject::PROJECTILE;
                                po->projectileType = Projectile::SBULLET;
                                po->active = true;
                                po->scale.Set(1, 1, 1);
                                po->pos.Set(other->pos.x, other->pos.y, other->pos.z);
                                Vector3 view = (skipper->getTarget()->pos - other->pos).Normalized();
                                if (go2->objectType == GameObject::BOSS)
                                {
                                    Boss *boss = (Boss *)*it2;

                                    switch (boss->bossType)
                                    {
                                    case Boss::FRILLEDSHARK:
                                    {
                                        FrilledShark *fs = (FrilledShark *)*it2;
                                        view = (fs->pos - other->pos).Normalized();
                                    }break;
                                    case Boss::GIANTCRAB:
                                    {
                                        GiantCrab *fs = (GiantCrab *)*it2;
                                        view = (fs->m_hitbox.m_position - other->pos).Normalized();
                                    }break;
                                    case Boss::ISOPOD:
                                    {
                                        Isopod *fs = (Isopod *)*it2;
                                        view = (fs->m_hitbox.m_position - other->pos).Normalized();
                                    }break;
                                    case Boss::GIANTSQUID:
                                    {
                                        Boss *another = (Boss *)*it2;
                                        view = (another->collision.m_position - other->pos).Normalized();
                                    }break;
                                    }
                                }
                                else
                                {
                                    view = (go2->pos - other->pos).Normalized();

                                }


                                po->vel.Set(view.x * 10, view.y * 10, view.z * 10);
                                other->setDebounceTimer(0);
                            }
                        }

                    }
                    else
                    {
                        other->setDebounceTimer(other->getDebounceTimer() + dt);
                    }

                    if (other->seaType == SeaCreature::FCRAB)
                    {
                        Minnow* other2 = (Minnow*)*it;

                        if (skipper->getTarget()->objectType == GameObject::BOSS)
                        {
                            for (std::vector<GameObject *>::iterator it2 = seaList.begin(); it2 != seaList.end(); ++it2)
                            {
                                GameObject* another = (GameObject*)*it2;
                                if (another->objectType == GameObject::BOSS)
                                {
                                    Boss* another = (Boss*)*it2;
                                    if (!(another->collision.m_position - other->pos).IsZero())
                                    {
                                        Vector3 Dir = (another->collision.m_position - other->pos).Normalized();
                                        other->vel += other->cohesion(another->collision.m_position, Dir) + other->alignment(0);
                                    }
                                }
                            }
                        }
                        else if (skipper->getTarget()->objectType == GameObject::SEACREATURE)
                        {
                            if (!(skipper->getTarget()->pos - other->pos).IsZero())
                            {
                                Vector3 Dir = (skipper->getTarget()->pos - other->pos).Normalized();
                                other->vel += other->cohesion(skipper->getTarget()->pos, Dir) + other->alignment(0);
                            }
                        }
                        else
                        {
                            float dist = (skipper->getTarget()->pos - other->pos).LengthSquared();
                            if (!(skipper->getTarget()->pos - other->pos).IsZero() && dist > 1000)
                            {
                                Vector3 Dir(0, 0, 0);
                                if (!(skipper->getTarget()->pos - other->pos).IsZero())
                                    Dir = (skipper->getTarget()->pos - other->pos).Normalize();
                                other->vel += other->cohesion(skipper->getTarget()->pos, Dir) + other->alignment(0);
                            }
                            else
                            {
                                Vector3 Dir(0, 0, 0);
                                if (!(skipper->getTarget()->pos - other->pos).IsZero())
                                    Dir = (skipper->getTarget()->pos - other->pos).Normalize();
                                other->vel = Dir * 30;

                            }
                        }

                        if (other->getDebounceTimer() > 3)//&& (collision(another->collision, go->pos)))
                        {
                            if (skipper->getTarget()->objectType == GameObject::BOSS)
                            {
                                for (std::vector<GameObject *>::iterator it2 = seaList.begin(); it2 != seaList.end(); ++it2)
                                {
                                    Boss* another = (Boss*)*it2;
                                    if (another->objectType == GameObject::BOSS && collision(another->collision, other->pos))
                                    {
                                        if (another->bossType == Boss::GIANTCRAB)
                                        {
                                            GiantCrab* gcrab = (GiantCrab*)*it2;

                                            if (gcrab->GetState() == GiantCrab::GRAB)
                                                gcrab->SetState(GiantCrab::STRAFE);

                                        }
                                        another->setHealth(another->getHealth() - skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));

                                        DamageText* text = FetchTO();
                                        text->setActive(true);
                                        text->setLastHitPos(another->pos);
                                        text->setLastDamage(skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));
                                        text->setScaleText(Vector3(0, 0, 0));
                                        other->setDebounceTimer(0);

                                        int random = 0;
                                        random = rand() % 10 + 1;
                                        if (random <= 2)
                                        {
                                            another->isstunned = true;
                                            another->StunDuration = 1.5f;
                                        }
                                    }
                                }
                            }
                            else if (skipper->getTarget()->objectType == GameObject::SEACREATURE)//(other->pos - skipper->getTarget()->pos).LengthSquared() <= 50)
                            {
                                for (std::vector<GameObject *>::iterator it2 = seaList.begin(); it2 != seaList.end(); ++it2)
                                {
                                    GameObject*go2 = (GameObject*)* it2;
                                    if (go2 == skipper->getTarget())
                                    {
                                        SeaCreature *another = (SeaCreature*)*it2;
                                        if (another->objectType == GameObject::SEACREATURE && (other->pos - skipper->getTarget()->pos).LengthSquared() <= 1000)
                                        {
                                            another->setHealth(another->getHealth() - skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));

                                            DamageText* text = FetchTO();
                                            text->setActive(true);
                                            text->setLastHitPos(another->pos);
                                            text->setLastDamage(skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));
                                            text->setScaleText(Vector3(0, 0, 0));
                                            other->setDebounceTimer(0);

                                            int random = 0;
                                            random = rand() % 10 + 1;
                                            if (random <= 5)
                                            {
                                                another->isstunned = true;
                                                another->StunDuration = 1.5f;
                                            }
                                        }
                                    }
                                }
                            }

                            else
                            {
                                other->setDebounceTimer(other->getDebounceTimer() + dt);
                            }
                        }
                    }
                    if (other->seaType == SeaCreature::CHIMERA)
                    {
                        Chimera *c = (Chimera *)*it;

                        for (std::vector<GameObject *>::iterator it2 = seaList.begin(); it2 != seaList.end(); ++it2)
                        {
                            GameObject *go2 = (GameObject *)*it2;
                            if (go2 == skipper->getTarget())
                            {
                                Vector3 displacement = go2->pos - other->pos;

                                c->rotate = -(Math::RadianToDegree(atan2(displacement.x, displacement.z)));
                                Vector3 direction = Vector3(Math::RadianToDegree(cos(Math::DegreeToRadian(c->rotate + 90))), displacement.y * 4, Math::RadianToDegree(sin(Math::DegreeToRadian(c->rotate + 90))));
                                c->vel = direction.Normalized() * 110;
                                if (other->getDebounceTimer() > 3)
                                {
                                    if (skipper->getTarget()->objectType == GameObject::BOSS)
                                    {
                                        for (std::vector<GameObject *>::iterator it2 = seaList.begin(); it2 != seaList.end(); ++it2)
                                        {
                                            Boss* another = (Boss*)*it2;
                                            if (another->objectType == GameObject::BOSS && (another->pos-other->pos).LengthSquared() < 1000)
                                            {
                                                if (another->bossType == Boss::GIANTCRAB)
                                                {
                                                    GiantCrab* gcrab = (GiantCrab*)*it2;

                                                    if (gcrab->GetState() == GiantCrab::GRAB)
                                                        gcrab->SetState(GiantCrab::STRAFE);

                                                }
                                                another->setHealth(another->getHealth() - skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));

                                                DamageText* text = FetchTO();
                                                text->setActive(true);
                                                text->setLastHitPos(another->pos);
                                                text->setLastDamage(skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));
                                                text->setScaleText(Vector3(0, 0, 0));
                                                other->setDebounceTimer(0);
                                            }
                                        }
                                    }
                                    else if (skipper->getTarget()->objectType == GameObject::SEACREATURE)//(other->pos - skipper->getTarget()->pos).LengthSquared() <= 50)
                                    {
                                        for (std::vector<GameObject *>::iterator it2 = seaList.begin(); it2 != seaList.end(); ++it2)
                                        {
                                            GameObject*go2 = (GameObject*)* it2;
                                            if (go2 == skipper->getTarget())
                                            {
                                                SeaCreature *another = (SeaCreature*)*it2;
                                                if (another->objectType == GameObject::SEACREATURE && (other->pos - skipper->getTarget()->pos).LengthSquared() <= 1000)
                                                {
                                                    another->setHealth(another->getHealth() - skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));

                                                    DamageText* text = FetchTO();
                                                    text->setActive(true);
                                                    text->setLastHitPos(another->pos);
                                                    text->setLastDamage(skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));
                                                    text->setScaleText(Vector3(0, 0, 0));
                                                    other->setDebounceTimer(0);

                                                }
                                            }
                                        }
                                    }
                                }

                                else
                                {
                                    other->setDebounceTimer(other->getDebounceTimer() + dt);
                                }
                            }
                        }
                    }
                }

                if (other->seaType == SeaCreature::CUTTLE && ((other->getDebounceTimer() == 0 || other->getDebounceTimer() >= 3)))
                {
                    for (std::vector<GameObject *>::iterator it2 = SharedData::GetInstance()->SD_CapturedList.begin(); it2 != SharedData::GetInstance()->SD_CapturedList.end(); ++it2)
                    {
                        SeaCreature* another = (SeaCreature*)* it2;
                        if (another->seaType != SeaCreature::CUTTLE)
                        {
                            if (another->getHealth() <= 90)
                            {
                                another->setHealth(another->getHealth() + 10);
                                cout << another->getHealth() << endl;

                                DamageText* text = FetchTO();
                                text->setActive(true);
                                text->setLastHitPos(another->pos + Vector3(0, 3.f, 0));
                                text->setLastDamage(10);
                                text->setScaleText(Vector3(0, 0, 0));
                                text->setIsEnemy(false);
                                text->setIsHeal(true);
                                text->setIsStamina(false);
                            }
                            else
                            {
                                another->setHealth(100);
                            }
                        }
                    }
                    if (skipper->getHealth() <= 500)
                    {
                        skipper->setHealth(skipper->getHealth() + 10);

                        DamageText* text = FetchTO();
                        text->setActive(true);
                        text->setLastHitPos(playerpos + fishVel + Vector3(0, 3.f, 0));
                        text->setLastDamage(10);
                        text->setScaleText(Vector3(0, 0, 0));
                        text->setIsEnemy(false);
                        text->setIsHeal(true);
                        text->setIsStamina(false);
                    }
                    else
                    {
                        skipper->setHealth(500);
                    }
                    other->setDebounceTimer(0.001);
                }
                else if (other->seaType == SeaCreature::CUTTLE)
                {
                    other->setDebounceTimer(other->getDebounceTimer() + dt);
                }
            }
        }
    }
}

void SceneSP3::UpdateStunned(double dt)
{
	for (std::vector<GameObject *>::iterator it = seaList.begin(); it != seaList.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if (go->objectType == GameObject::BOSS)
		{
			Boss* bo = (Boss*)* it;
			
			if (bo->isstunned == true)
			{
				bo->StunDuration -= dt;
				if (bo->StunDuration <= 0)
				{
					bo->isstunned = false;
				}
			}
		}
		else if (go->objectType == GameObject::SEACREATURE)
		{
			SeaCreature* bo = (SeaCreature*)* it;

			if (bo->isstunned == true)
			{
				bo->StunDuration -= dt;
				if (bo->StunDuration <= 0)
				{
					bo->isstunned = false;
				}
			}
		}
	}
}
float SceneSP3::LookAtPlayer(Vector3 playerpos, Vector3 otherpos)
{
    Vector3 initView(0, 0, 1);
    Vector3 wantView(playerpos - otherpos);
    Vector3 normal(0, 1, 0);

    if (!playerpos.IsZero() && !otherpos.IsZero())
        wantView.Normalize();

    float lookat = Math::RadianToDegree(acos(initView.Dot(wantView)));
    Vector3 Crossed = initView.Cross(wantView);
    if (Crossed.Dot(normal) < 0)
    {
        lookat *= -1;
    }
    return lookat;
}

void SceneSP3::RenderText(Mesh* mesh, std::string text, Color color)
{
    if (!mesh || mesh->textureID <= 0)
        return;

    glDisable(GL_DEPTH_TEST);
    glUniform1i(m_parameters[U_IS_GUI], 1);
    glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
    glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh->textureID);
    glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
    for (unsigned i = 0; i < text.length(); ++i)
    {
        Mtx44 characterSpacing;
        characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
        Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
        glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

        mesh->Render((unsigned)text[i] * 6, 6);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(m_parameters[U_IS_GUI], 0);
    glEnable(GL_DEPTH_TEST);
}

void SceneSP3::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
    if (!mesh || mesh->textureID <= 0)
        return;

    glDisable(GL_DEPTH_TEST);
    Mtx44 ortho;
    ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
    projectionStack.PushMatrix();
    projectionStack.LoadMatrix(ortho);
    viewStack.PushMatrix();
    viewStack.LoadIdentity();
    modelStack.PushMatrix();
    modelStack.LoadIdentity();
    modelStack.Translate(x, y, 0);
    modelStack.Scale(size, size, size);
    glUniform1i(m_parameters[U_IS_GUI], 1);
    glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
    glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh->textureID);
    glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
    for (unsigned i = 0; i < text.length(); ++i)
    {
        Mtx44 characterSpacing;
        characterSpacing.SetToTranslation(i * 0.3f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
        Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
        glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

        //cout << text[i] << endl;
        mesh->Render((unsigned)text[i] * 6, 6);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(m_parameters[U_IS_GUI], 0);
    modelStack.PopMatrix();
    viewStack.PopMatrix();
    projectionStack.PopMatrix();
    glEnable(GL_DEPTH_TEST);
}

void SceneSP3::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, const FontData &fd)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_IS_GUI], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	float currentOffset = 0;

	for (unsigned i = 0; i < text.length(); ++i)
	{
		currentOffset -= fd.GetWORatio((unsigned)text[i]);

		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(currentOffset, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);

		currentOffset += fd.GetBWRatio((unsigned)text[i]);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_IS_GUI], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneSP3::RenderMeshIn2D(Mesh *mesh, bool enableLight, float sizex , float sizey , float x, float y, float rot)
{
    glDisable(GL_DEPTH_TEST);

    Mtx44 ortho;
    ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
    projectionStack.PushMatrix();
    projectionStack.LoadMatrix(ortho);
    viewStack.PushMatrix();
    viewStack.LoadIdentity();
    modelStack.PushMatrix();
    modelStack.LoadIdentity();
    modelStack.Translate(x, y, 0);
    modelStack.Rotate(rot, 0, 0, 1);
    modelStack.Scale(sizex, sizey, 1);

    Mtx44 MVP, modelView, modelView_inverse_transpose;

    MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
    glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
    if (mesh->textureID > 0)
    {
        glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh->textureID);
        glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
    }
    else
    {
        glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
    }
    mesh->Render();
    if (mesh->textureID > 0)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    modelStack.PopMatrix();
    viewStack.PopMatrix();
    projectionStack.PopMatrix();
    glEnable(GL_DEPTH_TEST);
}

void SceneSP3::RenderMesh(Mesh *mesh, bool enableLight)
{
    Mtx44 MVP, modelView, modelView_inverse_transpose;

    //For Gpass/shadow textures
    if (m_renderPass == RENDER_PASS_PRE)
    {

        for (int i = 0; i < Mesh::MAX_TEXTURES; ++i)
        {
            if (mesh->textureArray[i] > 0)
            {
                glUniform1i(m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED + i], 1);
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
                glUniform1i(m_parameters[U_SHADOW_COLOR_TEXTURE + i], i);
            }
            else
            {
                glUniform1i(m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED + i], 0);
            }
        }

        Mtx44 lightDepthMVP = m_lightDepthProj * m_lightDepthView * modelStack.Top();
        glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP_GPASS], 1, GL_FALSE, &lightDepthMVP.a[0]);

        mesh->Render();
        return;
    }

    MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
    glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
    modelView = viewStack.Top() * modelStack.Top();
    glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

    if (enableLight && bLightEnabled)
    {
        glUniform1i(m_parameters[U_LIGHTENABLED], 1);
        modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
        glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

        //Shadow
        Mtx44 lightDepthMVP = m_lightDepthProj * m_lightDepthView * modelStack.Top();

        glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP], 1, GL_FALSE, &lightDepthMVP.a[0]);

        //load material
        glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
        glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
        glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
        glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
    }
    else
    {
        glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    }

    for (int i = 0; i < Mesh::MAX_TEXTURES; ++i)
    {
        if (mesh->textureArray[i] > 0)
        {
            glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
            glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
        }
        else
        {
            glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
        }
    }

    mesh->Render();
}

bool SceneSP3::GetKeyState(const unsigned char &key, const KeyStatuses &status)
{
	if (key >= unsigned char(255)) return false;
	if (status < 0 || status >= KEY_STATUS_TEMP) return false;

	return keyStates[status][key];
}

void SceneSP3::UpdateParticles(double dt)
{
    if (m_particleCount < 300)
    {
        ParticleObject* P_Bubble = GetParticle();
        P_Bubble->type = PARTICLEOBJECT_TYPE::P_BUBBLE;
        P_Bubble->scale.Set(5.0f, 5.0f, 5.0f);
        P_Bubble->vel.Set(Math::RandFloatMinMax(-3.f, 3.f), Math::RandFloatMinMax(0, 8.f), Math::RandFloatMinMax(-3.f, 3.f));
        P_Bubble->pos.Set(Math::RandFloatMinMax(-2000.f, 2000.f), Math::RandFloatMinMax(-2000.f, 2000.f), Math::RandFloatMinMax(-2000.f, 2000.f));
    }

    for (std::vector<GameObject *>::iterator it = seaList.begin(); it != seaList.end(); ++it)
    {
        GameObject* go = (GameObject*)*it;
        if (go->objectType != GameObject::SEACREATURE)
            continue;
        else
        {
            SeaCreature* sc = (SeaCreature*)*it;
            if (sc->active && sc->getHealth() <= 50 && sc->seaType != SeaCreature::DRONE)
            {
                ParticleObject* P_Vacuum = GetParticle();
                P_Vacuum->type = PARTICLEOBJECT_TYPE::P_VACUUM;
                P_Vacuum->scale.Set(5.0f, 5.0f, 5.0f);
                P_Vacuum->vel.Set(0, 0, 0);

                if (sc->seaType == SeaCreature::MINNOW)
                    P_Vacuum->pos.Set(sc->pos.x, sc->pos.y + 8.f, sc->pos.z);
                else
                    P_Vacuum->pos.Set(sc->pos.x, sc->pos.y + 15.f, sc->pos.z);
            }
        }
    }

	Vector3 c_pos = walkCam.GetPos();
	if (m_spCount < 50)
	{
		for (unsigned i = 0; i < 5; ++i)
		{
            ParticleObject* p = GetParticle();
            p->type = PARTICLEOBJECT_TYPE::P_PARTICLE;
			//p->scale.Set(5.0f, 5.0f, 5.0f);
			p->vel.Set(Math::RandFloatMinMax(-1.f, 1.f), Math::RandFloatMinMax(0, 2.f), Math::RandFloatMinMax(-1.f, 1.f));
			p->pos.Set(Math::RandFloatMinMax(c_pos.x - 50, c_pos.x + 50), Math::RandFloatMinMax(c_pos.y + 50, c_pos.y + 300), Math::RandFloatMinMax(c_pos.z - 50, c_pos.z + 50));
            m_spCount++;
		}
	}


    for (auto it : particleList)
    {
        ParticleObject* particle = (ParticleObject*)it;
		if (!particle->active)
			continue;

		switch (particle->type)
		{

        case PARTICLEOBJECT_TYPE::P_BUBBLE:
			{
				particle->vel += (0, 9.8f, 0) * (float)dt;
				particle->pos += particle->vel  * (float)dt * 10.f;
				particle->rotation = Math::RadianToDegree(atan2(playerpos.x - particle->pos.x, playerpos.z - particle->pos.z));

				if (particle->pos.y > 2000.f)
				{
					particle->active = false;
					m_particleCount--;
				}
			}
			break;
        case PARTICLEOBJECT_TYPE::P_VACUUM:
			{
                particle->rotation = Math::RadianToDegree(atan2(playerpos.x - particle->pos.x, playerpos.z - particle->pos.z));

                for (auto it : seaList)
                {
                    SeaCreature* sc = (SeaCreature*)it;
                    if (particle->pos != sc->pos)
                        particle->active = false;
                }
			}
			break;
        case PARTICLEOBJECT_TYPE::P_PARTICLE:
			{
				/*	particle->pos += particle->vel  * (float)dt * 10.f;
					particle->rotation = Math::RadianToDegree(atan2(playerpos.x - particle->pos.x, playerpos.z - particle->pos.z));*/
				UpdateSP(particle, dt);
			}
			break;
		}
     }
}

void SceneSP3::UpdateSP(ParticleObject* p, double dt)
{
	float range = 130;
	Vector3 displacment = walkCam.GetPos() - p->pos;

	if (displacment.LengthSquared() > range*range)
	{
		p->active = false;
		m_spCount-=1;
	}
	p->pos += p->vel*dt;
}

ParticleObject* SceneSP3::GetParticle()
{
    for (auto it : particleList)
    {
        ParticleObject *particle = (ParticleObject *)it;
        if (!particle->active)
        {
            particle->active = true;
            m_particleCount++;
            return particle;
        }
    }

    for (unsigned i = 0; i <= 1; ++i)
    {
        ParticleObject *particle = new ParticleObject(PARTICLEOBJECT_TYPE::P_BUBBLE);
        particleList.push_back(particle);
    }

    ParticleObject *particle = particleList.back();
    particle->active = true;


    return particle;
}

void SceneSP3::RenderParticles()
{
    for (auto it : particleList)
    {
        ParticleObject* particle = (ParticleObject*)it;
		if (!particle->active && particle->type == PARTICLEOBJECT_TYPE::P_VACUUM && (playerpos - particle->pos).LengthSquared() < 200 * 200)
        {
            modelStack.PushMatrix();
            modelStack.Translate(particle->pos.x, particle->pos.y, particle->pos.z);
            modelStack.Rotate(particle->rotation, 0, 1, 0);
            modelStack.Scale(particle->scale.x, particle->scale.y, particle->scale.z);
            RenderMesh(meshList[GEO_VACUUM], false);
            modelStack.PopMatrix();
            continue;
        }

        if (!particle->active)
            continue;
        else
        {
            switch (particle->type)
            {
            case PARTICLEOBJECT_TYPE::P_BUBBLE:
                modelStack.PushMatrix();
                modelStack.Translate(particle->pos.x, particle->pos.y, particle->pos.z);
                modelStack.Rotate(particle->rotation, 0, 1, 0);
                modelStack.Scale(particle->scale.x, particle->scale.y, particle->scale.z);
                RenderMesh(meshList[GEO_BUBBLE], false);
                modelStack.PopMatrix();
                break;
            case PARTICLEOBJECT_TYPE::P_PARTICLE:
                modelStack.PushMatrix();
                modelStack.Translate(particle->pos.x, particle->pos.y, particle->pos.z);
                modelStack.Scale(0.2, 0.2, 0.2);
                RenderMesh(meshList[GEO_CUBE], false);
                modelStack.PopMatrix();
                break;

            }
        }
    }

	modelStack.PushMatrix();
	modelStack.Translate(m_travelzonedown.m_position.x, m_travelzonedown.m_position.y+100, m_travelzonedown.m_position.z);
	modelStack.Rotate(vrotate, 0, 1, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_VORTEX], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(m_travelzoneup.m_position.x, m_travelzoneup.m_position.y+100, m_travelzoneup.m_position.z);
	modelStack.Rotate(vrotate, 0, 1, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_VORTEX], false);
	modelStack.PopMatrix();

}

void SceneSP3::RenderSquad()
{
	for (std::vector<GameObject *>::iterator it = SharedData::GetInstance()->SD_CapturedList.begin(); it != SharedData::GetInstance()->SD_CapturedList.end(); ++it)
	{
		SeaCreature *go = (SeaCreature *)*it;
		if (go->active == true)
		{
			switch (go->seaType)
			{
			case SeaCreature::MINNOW:
			{
				Minnow* fo = (Minnow*)go;
				float rotate = 0;
				rotate = Math::RadianToDegree(atan2(fo->vel.x, fo->vel.z));
				modelStack.PushMatrix();
				modelStack.Translate(fo->pos.x, fo->pos.y, fo->pos.z);
				modelStack.Rotate(rotate, 0, 1, 0);
				modelStack.Scale(fo->scale.x * 3, fo->scale.y * 3, fo->scale.z * 3);
				RenderMesh(meshList[GEO_CAP_MINNOW], false);
				modelStack.PopMatrix();
				break;
			}
			case SeaCreature::FCRAB:
			{
				Fcrab * c = (Fcrab*)go;
				float rotate = 0;
				rotate = Math::RadianToDegree(atan2(c->vel.x, c->vel.z));


				modelStack.PushMatrix();
				modelStack.Translate(c->pos.x, c->pos.y, c->pos.z);
				modelStack.Rotate(rotate, 0, 1, 0);
				modelStack.Scale(c->scale.x, c->scale.x, c->scale.x);
				RenderMesh(meshList[GEO_CAP_FCRABBODY], false);

				modelStack.PushMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(0.15, 0, -0.17);
				modelStack.Rotate(c->Crableg[0].rotate, 1, 0, 0);
				RenderMesh(meshList[GEO_CAP_FCRABLEG], false);//leg1
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(-0.15, 0, -0.17);
				modelStack.Rotate(c->Crableg[1].rotate, 1, 0, 0);
				RenderMesh(meshList[GEO_CAP_FCRABLEG], false);//leg2
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(-0.45, 0, -0.17);
				modelStack.Rotate(c->Crableg[2].rotate, 1, 0, 0);
				RenderMesh(meshList[GEO_CAP_FCRABLEG], false);//leg3
				modelStack.PopMatrix();
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Rotate(180, 0, 1, 0);

				modelStack.PushMatrix();
				modelStack.Translate(-0.15, 0, -0.17);
				modelStack.Rotate(c->Crableg[3].rotate, 1, 0, 0);
				RenderMesh(meshList[GEO_CAP_FCRABLEG], false);//leg4
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(0.15, 0, -0.17);
				modelStack.Rotate(c->Crableg[4].rotate, 1, 0, 0);
				RenderMesh(meshList[GEO_CAP_FCRABLEG], false);//leg5
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(0.45, 0, -0.17);
				modelStack.Rotate(c->Crableg[5].rotate, 1, 0, 0);
				RenderMesh(meshList[GEO_CAP_FCRABLEG], false);//leg6
				modelStack.PopMatrix();

				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(0.17, 0, -0.35);
				modelStack.Rotate(-55, 0, 1, 0);
				RenderMesh(meshList[GEO_CAP_FCRABCLAW], false);
				modelStack.PopMatrix();

				modelStack.PopMatrix();
				break;
			}
			case SeaCreature::PUFFER:
			{
				Pufferfish* fo = (Pufferfish*)go;
				float rotate = 0;
				rotate = Math::RadianToDegree(atan2(fo->vel.x, fo->vel.z));
				modelStack.PushMatrix();
				modelStack.Translate(fo->pos.x, fo->pos.y, fo->pos.z);
				modelStack.Rotate(rotate, 0, 1, 0);
				modelStack.Scale(fo->scale.x, fo->scale.y, fo->scale.z);
				RenderMesh(meshList[GEO_CAP_PUFFER], false);
				modelStack.PopMatrix();
				break;
			}
			case SeaCreature::CHIMERA:
			{
				Chimera * c = (Chimera*)go;
				modelStack.PushMatrix();
				modelStack.Translate(c->pos.x, c->pos.y, c->pos.z);
				modelStack.Rotate(-c->rotate, 0, 1, 0);
				modelStack.Scale(c->scale.x, c->scale.y, c->scale.z);
				RenderMesh(meshList[GEO_CAP_CHIMERA_BODY], false);

				modelStack.PushMatrix();
				modelStack.Translate(0, -0.05, 0.18);
				modelStack.Rotate(c->flip[0].rotation, 0, 0, 1);
				modelStack.Rotate(20, 1, 0, 0);
				RenderMesh(meshList[GEO_CAP_CHIMERA_FFLIP], false);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(0, -0.05, 0.18);
				modelStack.Rotate(-c->flip[1].rotation, 0, 0, 1);
				modelStack.Rotate(180, 0, 1, 0);
				modelStack.Rotate(-20, 1, 0, 0);
				RenderMesh(meshList[GEO_CAP_CHIMERA_FFLIP], false);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(0.08, -0.1, -0.3);
				modelStack.Rotate(c->flip[2].rotation, 0, 0, 1);
				RenderMesh(meshList[GEO_CAP_CHIMERA_BFLIP], false);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(-0.08, -0.1, -0.3);
				modelStack.Rotate(-c->flip[3].rotation, 0, 0, 1);
				modelStack.Rotate(180, 0, 1, 0);
				RenderMesh(meshList[GEO_CAP_CHIMERA_BFLIP], false);
				modelStack.PopMatrix();

				modelStack.PopMatrix();
				break;
			}
			case SeaCreature::CUTTLE:
			{
				Cuttlefish * c = (Cuttlefish*)go;
				modelStack.PushMatrix();
				glBlendFunc(1, 1);
				modelStack.Translate(c->pos.x, c->pos.y, c->pos.z);
				modelStack.Rotate(c->rotate, 0, 1, 0);
				modelStack.Scale(c->scale.z, c->scale.z, c->scale.z);
				RenderMesh(meshList[GEO_CAP_CUTTLE], false);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				modelStack.PopMatrix();
				break;
			}
			}

		}
	}
}

void SceneSP3::RenderFO(SeaCreature *fo)
{
    switch (fo->seaType)
    {
    case SeaCreature::MINNOW:
    {
        Minnow* minnow = (Minnow*)fo;
        if (!minnow->getisLeader())
        {
            float rotate = 0;
            rotate = Math::RadianToDegree(atan2(fo->vel.x, fo->vel.z));
            modelStack.PushMatrix();
            modelStack.Translate(fo->pos.x, fo->pos.y, fo->pos.z);
            modelStack.Rotate(rotate, 0, 1, 0);
            modelStack.Scale(fo->scale.x, fo->scale.y, fo->scale.z);
            RenderMesh(meshList[GEO_MINNOW], false);
            modelStack.PopMatrix();
        }
        else
        {
            float rotate = 0;
            rotate = Math::RadianToDegree(atan2(fo->vel.x, fo->vel.z));
            modelStack.PushMatrix();
            modelStack.Translate(fo->pos.x, fo->pos.y, fo->pos.z);
            modelStack.Rotate(rotate, 0, 1, 0);
            modelStack.Scale(fo->scale.x * 3, fo->scale.y * 3, fo->scale.z * 3);
            RenderMesh(meshList[GEO_MINNOW], false);
            modelStack.PopMatrix();
        }
        break;
    }
    case SeaCreature::PUFFER:
    {
        float rotate = 0;
        rotate = Math::RadianToDegree(atan2(fo->vel.x, fo->vel.z));
        modelStack.PushMatrix();
        modelStack.Translate(fo->pos.x, fo->pos.y, fo->pos.z);
        modelStack.Rotate(rotate, 0, 1, 0);
        modelStack.Scale(fo->scale.x, fo->scale.y, fo->scale.z);
        RenderMesh(meshList[GEO_PUFFER], false);
        modelStack.PopMatrix();
        break;
    }
    case SeaCreature::FCRAB:
    {
        Fcrab* c = (Fcrab*)fo;
        float rotate = 0;
        rotate = Math::RadianToDegree(atan2(c->vel.x, c->vel.z));

        modelStack.PushMatrix();
        modelStack.Translate(c->pos.x, c->pos.y, c->pos.z);
        modelStack.Rotate(rotate, 0, 1, 0);
        modelStack.Scale(c->scale.x, c->scale.x, c->scale.x);
        RenderMesh(meshList[GEO_FCRABBODY], false);

        modelStack.PushMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(0.15f, 0, -0.17f);
        modelStack.Rotate(c->Crableg[0].rotate, 1, 0, 0);
        RenderMesh(meshList[GEO_FCRABLEG], false);//leg1
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(-0.15f, 0, -0.17f);
        modelStack.Rotate(c->Crableg[1].rotate, 1, 0, 0);
        RenderMesh(meshList[GEO_FCRABLEG], false);//leg2
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(-0.45f, 0, -0.17f);
        modelStack.Rotate(c->Crableg[2].rotate, 1, 0, 0);
        RenderMesh(meshList[GEO_FCRABLEG], false);//leg3
        modelStack.PopMatrix();
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Rotate(180, 0, 1, 0);

        modelStack.PushMatrix();
        modelStack.Translate(-0.15f, 0, -0.17f);
        modelStack.Rotate(c->Crableg[3].rotate, 1, 0, 0);
        RenderMesh(meshList[GEO_FCRABLEG], false);//leg4
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(0.15f, 0, -0.17f);
        modelStack.Rotate(c->Crableg[4].rotate, 1, 0, 0);
        RenderMesh(meshList[GEO_FCRABLEG], false);//leg5
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(0.45f, 0, -0.17f);
        modelStack.Rotate(c->Crableg[5].rotate, 1, 0, 0);
        RenderMesh(meshList[GEO_FCRABLEG], false);//leg6
        modelStack.PopMatrix();

        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(0.17f, 0, -0.35f);
        modelStack.Rotate(-55, 0, 1, 0);
        RenderMesh(meshList[GEO_FCRABCLAW], false);
        modelStack.PopMatrix();

        modelStack.PopMatrix();
        break;
    }
    case SeaCreature::CUTTLE:
    { 
        Cuttlefish* c = (Cuttlefish*)fo;
        modelStack.PushMatrix();
        glBlendFunc(1, 1);
        modelStack.Translate(c->pos.x, c->pos.y, c->pos.z);
        modelStack.Rotate(c->rotate, 0, 1, 0);
        modelStack.Scale(c->scale.z, c->scale.z, c->scale.z);
        RenderMesh(meshList[GEO_CUTTLE], true);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        modelStack.PopMatrix();
        break;
    }
    case SeaCreature::CHIMERA:
    {
        Chimera* c = (Chimera*)fo;
        modelStack.PushMatrix();
        modelStack.Translate(c->pos.x, c->pos.y, c->pos.z);
        modelStack.Rotate(-c->rotate, 0, 1, 0);
        modelStack.Scale(c->scale.x, c->scale.y, c->scale.z);
        RenderMesh(meshList[GEO_CHIMERA_BODY], false);

        modelStack.PushMatrix();
        modelStack.Translate(0, -0.05f, 0.18f);
        modelStack.Rotate(c->flip[0].rotation, 0, 0, 1);
        modelStack.Rotate(20, 1, 0, 0);
        RenderMesh(meshList[GEO_CHIMERA_FFLIP], false);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(0, -0.05f, 0.18f);
        modelStack.Rotate(-c->flip[1].rotation, 0, 0, 1);
        modelStack.Rotate(180, 0, 1, 0);
        modelStack.Rotate(-20, 1, 0, 0);
        RenderMesh(meshList[GEO_CHIMERA_FFLIP], false);
        modelStack.PopMatrix();


        modelStack.PushMatrix();
        modelStack.Translate(0.08f, -0.1f, -0.3f);
        modelStack.Rotate(c->flip[2].rotation, 0, 0, 1);
        RenderMesh(meshList[GEO_CHIMERA_BFLIP], false);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(-0.08f, -0.1f, -0.3f);
        modelStack.Rotate(-c->flip[3].rotation, 0, 0, 1);
        modelStack.Rotate(180, 0, 1, 0);
        RenderMesh(meshList[GEO_CHIMERA_BFLIP], false);
        modelStack.PopMatrix();

        modelStack.PopMatrix();
        break;
    }
	case SeaCreature::DRONE:
	{
		Drone* c = (Drone*)fo;

		float rotate = 0;
		rotate = Math::RadianToDegree(atan2(c->vel.x, c->vel.z));
		modelStack.PushMatrix();
		glBlendFunc(1, 1);
		modelStack.Translate(c->pos.x, c->pos.y, c->pos.z);
		modelStack.Rotate(rotate+180, 0, 1, 0);
		modelStack.Scale(c->scale.z, c->scale.z, c->scale.z);
		RenderMesh(meshList[GEO_ISOPOD_DRONE], false);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		modelStack.PopMatrix();
		break;
	}
    }
}

void SceneSP3::RenderCoral(Coral *co)
{
    if (co->coralType == Coral::HEALTH)
    {
        modelStack.PushMatrix();
        modelStack.Translate(co->pos.x, co->pos.y, co->pos.z);
        modelStack.Scale(co->scale.x, co->scale.y, co->scale.z);
        RenderMesh(meshList[GEO_CORAL], false);
        modelStack.PopMatrix();
    }
    else if (co->coralType == Coral::STAMINA)
    {
        modelStack.PushMatrix();
        modelStack.Translate(co->pos.x, co->pos.y, co->pos.z);
        modelStack.Scale(co->scale.x, co->scale.y, co->scale.z);
        RenderMesh(meshList[GEO_CORAL2], false);
        modelStack.PopMatrix();
    }
}

void SceneSP3::RenderPO(Projectile *po)
{
    if (po->projectileType == Projectile::PBULLET || po->projectileType == Projectile::SBULLET)
    {
        modelStack.PushMatrix();
        modelStack.Translate(po->pos.x, po->pos.y, po->pos.z);
        modelStack.Scale(po->scale.x, po->scale.y, po->scale.z);

        float angle[2];
        angle[0] = Math::RadianToDegree(atan2(-po->vel.z, po->vel.x));
        angle[1] = Math::RadianToDegree(asin(po->vel.y / po->vel.Length()));
        modelStack.Rotate(angle[0], 0, 1, 0);
        modelStack.Rotate(angle[1], 0, 0, 1);

        RenderMesh(meshList[GEO_LASER], false);
        modelStack.PopMatrix();
    }
    else if (po->projectileType == Projectile::INK)
    {
        modelStack.PushMatrix();
        modelStack.Translate(po->pos.x, po->pos.y, po->pos.z);
        modelStack.Scale(po->scale.x, po->scale.y, po->scale.z);
        RenderMesh(meshList[GEO_BALL], false);
        modelStack.PopMatrix();
    }
}

void SceneSP3::RenderLoop()
{
    for (std::vector<GameObject *>::iterator it = seaList.begin(); it != seaList.end(); ++it)
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
                RenderFO(p);
                break;
            }
            case SeaCreature::FCRAB:
            {
                Fcrab * c = (Fcrab*)*it;
                RenderFO(c);
                break;
            }
            case SeaCreature::CHIMERA:
            {
                Chimera * c = (Chimera*)*it;
                RenderFO(c);
                break;
            }
            case SeaCreature::CUTTLE:
            {
                Cuttlefish * c = (Cuttlefish*)*it;
                RenderFO(c);
                break;
            }
			case SeaCreature::DRONE:
			{
				Drone * c = (Drone*)*it;
				RenderFO(c);
				break;
			}
            }
        }
    }

    for (std::vector<GameObject *>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
    {
        GameObject *go = (GameObject*)*it;

        if (!go->active)
            continue;
        else if (go->objectType == GameObject::PROJECTILE)
        {
            Projectile *po = (Projectile*)*it;
            if (po->active)
            {
                RenderPO(po);
            }
        }
    }

    for (std::vector<GameObject *>::iterator it = coralList.begin(); it != coralList.end(); ++it)
    {
        GameObject *go = (GameObject*)*it;

        if (!go->active)
            continue;
        else if (go->objectType == GameObject::CORAL)
        {
            Coral *co = (Coral*)*it;
            if (co->active)
            {
                RenderCoral(co);
            }
        }
    }
	RenderSquad();

}

void SceneSP3::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// PRE RENDER PASS
	RenderPassGPass();

	// MAIN RENDER PASS
	RenderPassMain();
}

void SceneSP3::RenderDeathScreen()
{
    if (skipper->getIsDead())
    {
        isGamePaused = true;

        static bool b_DS_UPState = false;
        if (GetKeyState('W', KEY_STATUS_DOWN) ||
            GetKeyState(VK_UP, KEY_STATUS_DOWN))
        {
            if (DeathSelect != DEATHSELECT::RESPAWN)
            {
                int currentDeathSelect = static_cast<int>(DeathSelect);
                currentDeathSelect--;
                DeathSelect = static_cast<DEATHSELECT>(currentDeathSelect);
            }
        }

        static bool b_DS_DOWNState = false;

        if (GetKeyState('S', KEY_STATUS_DOWN) ||
            GetKeyState(VK_DOWN, KEY_STATUS_DOWN))
        {
            if (DeathSelect != DEATHSELECT::QUIT)
            {
                int currentDeathSelect = static_cast<int>(DeathSelect);
                currentDeathSelect++;
                DeathSelect = static_cast<DEATHSELECT>(currentDeathSelect);
            }
        }

        glBlendFunc(GL_ONE, GL_ONE);
        RenderMeshIn2D(meshList[GEO_TLAYER], false, 35.f, 30.f);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
        RenderMeshIn2D(meshList[GEO_TDIED], false, 30.f, 12.f, 0.f, 20.0f);

        switch (DeathSelect)
        {
        case RESPAWN:
        {
            RenderMeshIn2D(meshList[GEO_TBORDER], false, 33.f, 5.f, 0.f, 1.0f);
            RenderMeshIn2D(meshList[GEO_TRESPAWN], false, 50.f, 6.f, 0.f, 0.0f);
			RenderMeshIn2D(meshList[GEO_TMENU], false, 20.f, 5.f, 0.f, -10.0f);
            RenderMeshIn2D(meshList[GEO_TQUIT], false, 20.f, 5.f, 0.f, -20.0f);
            break;
        }
        case MENU:
        {
            RenderMeshIn2D(meshList[GEO_TBORDER], false, 33.f, 5.f, 0.f, -11.0f);
            RenderMeshIn2D(meshList[GEO_TMENU], false, 30.f, 5.f, 0.f, -10.0f);
            RenderMeshIn2D(meshList[GEO_TRESPAWN], false, 40.f, 5.f, 0.f, 0.0f);
            RenderMeshIn2D(meshList[GEO_TQUIT], false, 20.f, 5.f, 0.f, -20.0f);
            break;
        }
        case QUIT:
        {
            RenderMeshIn2D(meshList[GEO_TBORDER], false, 33.f, 5.f, 0.f, -21.0f);
            RenderMeshIn2D(meshList[GEO_TQUIT], false, 25.f, 5.f, 0.f, -20.0f);
            RenderMeshIn2D(meshList[GEO_TMENU], false, 25.f, 5.f, 0.f, -10.0f);
            RenderMeshIn2D(meshList[GEO_TRESPAWN], false, 40.f, 5.f, 0.f, 0.0f);
            break;
        }
        }
    }

}

void SceneSP3::UpdateDeathScreen(double dt)
{
    if (skipper->getIsDead())
    {
        switch (DeathSelect)
        {
        case RESPAWN:
        {
            if (GetKeyState(13, KEY_STATUS_DOWN))
            {
                isGamePaused = false;
                Application::sceneManager->SetPreviousScene(Application::sceneManager->GetCurrentScene());
                Application::sceneManager->SetCurrentScene(Application::sceneManager->GetLoadingScreen());
            }
            break;
        }
        case MENU:
        {
            RenderMeshIn2D(meshList[GEO_TBORDER], false, 33.f, 5.f, 0.f, -11.0f);
            RenderMeshIn2D(meshList[GEO_TMENU], false, 25.f, 5.f, 0.f, -10.0f);
			RenderMeshIn2D(meshList[GEO_TRESPAWN], false, 30.f, 4.f, 0.f, 0.0f);
			RenderMeshIn2D(meshList[GEO_TQUIT], false, 20.f, 5.f, 0.f, -20.0f);
            if (GetKeyState(13, KEY_STATUS_DOWN))
            {
                isGamePaused = false;
                Application::sceneManager->SetPreviousScene(Application::sceneManager->GetCurrentScene());
                Application::sceneManager->SetMenuScene(new SceneMenu());
                Application::sceneManager->GetMenuScreen()->Init();
                Application::sceneManager->SetCurrentScene(Application::sceneManager->GetMenuScreen());
            }
            break;
        }
        case QUIT:
        {
            RenderMeshIn2D(meshList[GEO_TBORDER], false, 33.f, 5.f, 0.f, -21.0f);
            RenderMeshIn2D(meshList[GEO_TQUIT], false, 25.f, 5.f, 0.f, -20.0f);
			RenderMeshIn2D(meshList[GEO_TMENU], false, 20.f, 5.f, 0.f, -10.0f);
			RenderMeshIn2D(meshList[GEO_TRESPAWN], false, 30.f, 4.f, 0.f, 0.0f);
            if (GetKeyState(13, KEY_STATUS_DOWN))
            {
                SharedData::GetInstance()->SD_QuitGame = true;
                isGamePaused = false;
            }
            break;
        }
        }
    }
}

void SceneSP3::RenderObjectiveHUD()
{
    if (SharedData::GetInstance()->SD_ObjectiveTabOn)
    {
        if (sliderTranslate <= -65)
            sliderTranslate += 5.f;
    }
    else
    {
        if (sliderTranslate >= -100)
            sliderTranslate -= 5.f;
    }

    glBlendFunc(GL_ONE, GL_ONE);
    RenderMeshIn2D(meshList[GEO_TLAYER], false, 20, 20, sliderTranslate);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    if (SharedData::GetInstance()->SD_BossDead1)
        RenderMeshIn2D(meshList[GEO_TDIED], false, 20, 5, sliderTranslate, 15);
    if (SharedData::GetInstance()->SD_BossDead2)
        RenderMeshIn2D(meshList[GEO_TDIED], false, 20, 5, sliderTranslate, 5);
    if (SharedData::GetInstance()->SD_BossDead3)
        RenderMeshIn2D(meshList[GEO_TDIED], false, 20, 5, sliderTranslate, -5);
    if (SharedData::GetInstance()->SD_BossDead4)
        RenderMeshIn2D(meshList[GEO_TDIED], false, 20, 5, sliderTranslate, -15);
}

void SceneSP3::RenderCoralHUD()
{
    RenderMeshIn2D(meshList[GEO_CORAL_HUD], false, 5, 5, 35, -44);
    std::ostringstream ss; 
    ss << skipper->getHealthPackCount();
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.7f, 0.7f, 0.7f), 4, 59, 5);

    RenderMeshIn2D(meshList[GEO_CORAL2_HUD], false, 5, 5, 40, -54);
    ss.str("");
    ss << skipper->getStaminaPackCount();
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.7f, 0.7f, 0.7f), 4, 61.5f, 0);
}

void SceneSP3::RenderSquadHUD()
{
    RenderMeshIn2D(meshList[GEO_MINNOW_HUD], false, 5, 5, 75, 30);
    std::ostringstream ss;
    ss << g_countCapturedMinnow;
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.7f, 0.7f, 0.7f), 4, 79, 42);

    RenderMeshIn2D(meshList[GEO_PUFFER_HUD], false, 5, 5, 75, 20);
    ss.str("");
    ss << g_countCapturedPuffer;
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.7f, 0.7f, 0.7f), 4, 79, 37);

    RenderMeshIn2D(meshList[GEO_CRAB_HUD], false, 5, 5, 75, 10);
    ss.str("");
    ss << g_countCapturedCrab;
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.7f, 0.7f, 0.7f), 4, 79, 32);

    RenderMeshIn2D(meshList[GEO_CUTTLE_HUD], false, 5, 5, 75, 0);
    ss.str("");
    ss << g_countCapturedCuttle;
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.7f, 0.7f, 0.7f), 4, 79, 27);

    RenderMeshIn2D(meshList[GEO_CHIMERA_HUD], false, 5, 5, 75, -10);
    ss.str("");
    ss << g_countCapturedChimera;
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.7f, 0.7f, 0.7f), 4, 79, 22);
}

void SceneSP3::RenderVictoryScreen()
{
    RenderMeshIn2D(meshList[GEO_TVICTORY], false, 80, 60);
}

void SceneSP3::RenderCreditsScreen()
{
    RenderMeshIn2D(meshList[GEO_TCREDITS], false, 80, 60);
}

void SceneSP3::RenderHUD()
{
	glUniform1i(m_parameters[U_FOG_ENABLE], 0);

    if (GetKeyState(VK_NUMPAD5, KEY_STATUS_DOWN))
    {
        SharedData::GetInstance()->SD_BossDead1 = true;
        SharedData::GetInstance()->SD_BossDead2 = true;
        SharedData::GetInstance()->SD_BossDead3 = true;
        SharedData::GetInstance()->SD_BossDead4 = true;
    }

    float barscale = 50.f;
    for (std::vector<GameObject *>::iterator it = seaList.begin(); it != seaList.end(); ++it)
    {
        GameObject* go = (GameObject*)*it;
        if (go->objectType != GameObject::BOSS)
            continue;
        else
        {
            Boss* bo = (Boss*)*it;
			std::ostringstream boss_name;

            switch (bo->bossType)
            {
            case Boss::GIANTSQUID:
            {
                if ((bo->pos - playerpos).LengthSquared() < g_distFromGiantSquid * g_distFromGiantSquid && !SharedData::GetInstance()->SD_BossDead1)
                {				
					boss_name << "giant squid";
					RenderTextOnScreen(meshList[GEO_TEXT], boss_name.str(), Color(0.7, 0.7, 0.7), 5, 34, 49, fontData);

                    float percentage = (100.f / 1000.f) * (float)bo->getHealth();
                    float healthscale = (barscale / 100) * percentage;
                    float healthtranslate = (50.f / 100.f) * (100.f - percentage);
                    RenderMeshIn2D(meshList[GEO_HUD_HEALTHBAR], false, 50.0f, 12.0f, 0.f, 50.0f);
                    RenderMeshIn2D(meshList[GEO_HUD_BOSSHEALTH], false, healthscale, 10.0f, 0.f - healthtranslate, 50.0f);
                }
                break;
			case Boss::GIANTCRAB:
			{
                if ((bo->pos - playerpos).LengthSquared() < 600 * 600 && !SharedData::GetInstance()->SD_BossDead2)
				{
					boss_name << "spider crab";
					RenderTextOnScreen(meshList[GEO_TEXT], boss_name.str(), Color(0.7, 0.7, 0.7), 5, 34, 49, fontData);

					float percentage = (100.f / 1000.f) * (float)bo->getHealth();
					float healthscale = (barscale / 100) * percentage;
					float healthtranslate = (50.f / 100.f) * (100.f - percentage);
					RenderMeshIn2D(meshList[GEO_HUD_HEALTHBAR], false, 50.0f, 12.0f, 0.f, 50.0f);
					RenderMeshIn2D(meshList[GEO_HUD_BOSSHEALTH], false, healthscale, 10.0f, 0.f - healthtranslate, 50.0f);
				}
			}
			break;
			case Boss::FRILLEDSHARK:
			{
                if ((bo->pos - playerpos).LengthSquared() < 800 * 800 && !SharedData::GetInstance()->SD_BossDead3)
				{
					boss_name << "frilled shark";
					RenderTextOnScreen(meshList[GEO_TEXT], boss_name.str(), Color(0.7, 0.7, 0.7), 5, 34, 49, fontData);

					float percentage = (100.f / 1000.f) * (float)bo->getHealth();
					float healthscale = (barscale / 100) * percentage;
					float healthtranslate = (50.f / 100.f) * (100.f - percentage);
					RenderMeshIn2D(meshList[GEO_HUD_HEALTHBAR], false, 50.0f, 12.0f, 0.f, 50.0f);
					RenderMeshIn2D(meshList[GEO_HUD_BOSSHEALTH], false, healthscale, 10.0f, 0.f - healthtranslate, 50.0f);
				}
			}
			break;
			case Boss::ISOPOD:
			{
                if ((bo->pos - playerpos).LengthSquared() < 800 * 800 && !SharedData::GetInstance()->SD_BossDead4)
				{
					boss_name << "great isopod";
					RenderTextOnScreen(meshList[GEO_TEXT], boss_name.str(), Color(0.7, 0.7, 0.7), 5, 34, 49, fontData);

					float percentage = (100.f / 1000.f) * (float)bo->getHealth();
					float healthscale = (barscale / 100) * percentage;
					float healthtranslate = (50.f / 100.f) * (100.f - percentage);
					RenderMeshIn2D(meshList[GEO_HUD_HEALTHBAR], false, 50.0f, 12.0f, 0.f, 50.0f);
					RenderMeshIn2D(meshList[GEO_HUD_BOSSHEALTH], false, healthscale, 10.0f, 0.f - healthtranslate, 50.0f);
				}
			}
			break;
            }
            }
        }
    }

    RenderObjectiveHUD();

    if (SharedData::GetInstance()->SD_TurnFPSOn)
    {
        std::ostringstream ss;
        ss.precision(3);
        ss << "FPS: " << fps;
        RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 2, 3);
    }

    if (SharedData::GetInstance()->SD_PauseMenu)
    {
        RenderPauseScreen();
        isGamePaused = true;
    }

    RenderDeathScreen();

    if (!SharedData::GetInstance()->SD_DoneTutorial)
    {
        isGamePaused = true;

        if (SharedData::GetInstance()->SD_SceneLoaded && SharedData::GetInstance()->SD_ContinueInstruction1 && SharedData::GetInstance()->SD_ContinueInstruction2)
        {
            SharedData::GetInstance()->SD_DoneTutorial = true;
            isGamePaused = false;
        }
        else
        {
            glBlendFunc(GL_ONE, GL_ONE);
            RenderMeshIn2D(meshList[GEO_TLAYER], false, 45, 50);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        if (GetKeyState(VK_RETURN, KEY_STATUS_DOWN) && !SharedData::GetInstance()->SD_ContinueInstruction1)
            SharedData::GetInstance()->SD_ContinueInstruction1 = true;
        else if (GetKeyState(VK_RETURN, KEY_STATUS_DOWN) && SharedData::GetInstance()->SD_ContinueInstruction1 && !SharedData::GetInstance()->SD_ContinueInstruction2)
        {
            SharedData::GetInstance()->SD_ContinueInstruction2 = true;
        }

        if (!SharedData::GetInstance()->SD_ContinueInstruction1)
            RenderMeshIn2D(meshList[GEO_INSTRUCTION], false, 45, 50);

        if (SharedData::GetInstance()->SD_ContinueInstruction1 && !SharedData::GetInstance()->SD_ContinueInstruction2)
            RenderMeshIn2D(meshList[GEO_INSTRUCTION2], false, 45, 50);
    }

    RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 10.0f, 10.0f);
    RenderSquadHUD();
    RenderMinimap();
    RenderCoralHUD();

    // Victory & Credits Screen
    if (SharedData::GetInstance()->SD_BossDead1 &&
        SharedData::GetInstance()->SD_BossDead2 &&
        SharedData::GetInstance()->SD_BossDead3 &&
        SharedData::GetInstance()->SD_BossDead4)
    {
        isGamePaused = true;

        if (SharedData::GetInstance()->SD_ContinueVictoryScreen)
            RenderCreditsScreen();

        if (!SharedData::GetInstance()->SD_ContinueVictoryScreen)
            RenderVictoryScreen();

        if (GetKeyState(VK_RETURN, KEY_STATUS_DOWN) && !SharedData::GetInstance()->SD_ContinueVictoryScreen)
            SharedData::GetInstance()->SD_ContinueVictoryScreen = true;
        else if (GetKeyState(VK_RETURN, KEY_STATUS_DOWN) && SharedData::GetInstance()->SD_ContinueVictoryScreen && !SharedData::GetInstance()->SD_ContinueCreditsScreen)
            SharedData::GetInstance()->SD_ContinueCreditsScreen = true;

        if (SharedData::GetInstance()->SD_BossDead1 &&
            SharedData::GetInstance()->SD_BossDead2 &&
            SharedData::GetInstance()->SD_BossDead3 &&
            SharedData::GetInstance()->SD_BossDead4 &&
            SharedData::GetInstance()->SD_ContinueVictoryScreen &&
            SharedData::GetInstance()->SD_ContinueCreditsScreen)
        {
            isGamePaused = false;
            SharedData::GetInstance()->SD_BossDead1 = false;
            SharedData::GetInstance()->SD_BossDead2 = false;
            SharedData::GetInstance()->SD_BossDead3 = false;
            SharedData::GetInstance()->SD_BossDead4 = false;
            SharedData::GetInstance()->SD_ToMenu = true;
        }
    }

	glUniform1i(m_parameters[U_FOG_ENABLE], 1);
}

void SceneSP3::RenderMinimap()
{
	const float scale = 0.08f;

	Vector2 mPos;
	mPos.Set(80 - 16 - 4, -60 + 16 + 4);

	float angle = 90 + Math::RadianToDegree(atan2(walkCam.GetDir().z, walkCam.GetDir().x));

    float HealthPercentage = (100.f / 500.f) * (float)skipper->getHealth();
    float degree = 180.f * ((float)HealthPercentage / 100.f);

    for (int i = 0; i < degree; i += 2)
        RenderMeshIn2D(meshList[GEO_PLAYERHEALTH], false, 20, 20, mPos.x, mPos.y, -90 + i);

	float degree2 = 180.f * float(skipper->stamina/100.f);

	for (int i = 0; i < degree2; i += 2)
		RenderMeshIn2D(meshList[GEO_PLAYERSTAMINA], false, 20, 20, mPos.x, mPos.y, -90 - i);

	RenderMeshIn2D(meshList[GEO_MINIMAP], false, 20, 20,
		mPos.x, mPos.y, angle);
	
    for (auto it : seaList)
	{
		if (!it->active) continue;

		Vector3 p1 = it->pos - playerpos;
		{
			Mtx44 rot;
			rot.SetToRotation(angle, 0, 1, 0);
			p1 = rot * p1;
		}

		Vector2 p2;
		p2.Set(p1.x * scale, -p1.z * scale);

		switch (it->objectType)
		{
		case GameObject::SEACREATURE:
		{
			auto o1 = (SeaCreature*)it;
			GEOMETRY_TYPE g = GEO_MINIMAP_MINNOW;

			switch (o1->seaType)
			{
			case SeaCreature::MINNOW:
				g = GEO_MINIMAP_MINNOW;
				break;

			case SeaCreature::CUTTLE:
				g = GEO_MINIMAP_CUTTLE;
				break;

			case SeaCreature::FCRAB:
				g = GEO_MINIMAP_CRAB;
				break;

			case SeaCreature::CHIMERA:
				g = GEO_MINIMAP_GHOSTSHARK;
				break;

			case SeaCreature::PUFFER:
				g = GEO_MINIMAP_PUFFER;
				break;

			//More SeaCreatures goes below here.

			}

			const float range = 14.5f;
			bool inRange = true;

			if (p2.LengthSquared() > range * range)
			{
				p2.Normalize() *= range;
				inRange = false;
			}

			Vector2 r = mPos + p2;

            RenderMeshIn2D(meshList[g], false, inRange ? 1.f : 0.8f, inRange ? 1.f : 0.8f, r.x, r.y);

		} break;

		case GameObject::BOSS:
		{
			const float range = 14.9f;
			bool inRange = true;

			if (p2.LengthSquared() > range * range)
			{
				p2.Normalize() *= range;
				inRange = false;
			}

			Vector2 r = mPos + p2;

            RenderMeshIn2D(meshList[GEO_MINIMAP_BOSS], false, inRange ? 1.5f : 1.2f, inRange ? 1.5f : 1.2f, r.x, r.y);

		} break;

		//More ObjectTypes goes below here.

		}
	}

	{
		Vector3 p1;

		if (SharedData::GetInstance()->SD_CurrentArea != (unsigned)SharedData::A_TUTORIAL)
		{
			p1 = m_travelzoneup.m_position - playerpos;
			{
				Mtx44 rot;
				rot.SetToRotation(angle, 0, 1, 0);
				p1 = rot * p1;
			}

			Vector2 p2;
			p2.Set(p1.x * scale, -p1.z * scale);

			const float range = 14.5f;

			if (p2.LengthSquared() > range * range)
				p2.Normalize() *= range;

			Vector2 r = mPos + p2;

			RenderMeshIn2D(meshList[GEO_MINIMAP_ARROW_UP], false, 1.f, 1.f, r.x, r.y);
		}

		if (SharedData::GetInstance()->SD_CurrentArea != (unsigned)SharedData::A_NIGHTMARETRENCH)
		{
			p1 = m_travelzonedown.m_position - playerpos;
			{
				Mtx44 rot;
				rot.SetToRotation(angle, 0, 1, 0);
				p1 = rot * p1;
			}

			Vector2 p2;
			p2.Set(p1.x * scale, -p1.z * scale);

			const float range = 14.5f;

			if (p2.LengthSquared() > range * range)
				p2.Normalize() *= range;

			Vector2 r = mPos + p2;

			RenderMeshIn2D(meshList[GEO_MINIMAP_ARROW_DOWN], false, 1.f, 1.f, r.x, r.y);
		}
	}

    RenderMeshIn2D(meshList[GEO_MINIMAP_AVATAR], false, 3.f, 3.f,
		mPos.x, mPos.y);
}

void SceneSP3::RenderPauseScreen()
{
    glBlendFunc(GL_ONE, GL_ONE);
    RenderMeshIn2D(meshList[GEO_TLAYER], false, 35.f, 30.f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    RenderMeshIn2D(meshList[GEO_TPAUSE], false, 35.f, 20.f, 0.f, 20.0f);

    switch (pauseChoice)
    {
    case 0:
    {
        RenderMeshIn2D(meshList[GEO_TBORDER], false, 33.f, 5.f, 0.f, 1.0f);
        RenderMeshIn2D(meshList[GEO_TRESUME], false, 25.f, 4.f, 0.f, 1.0f);
        RenderMeshIn2D(meshList[GEO_TMENU], false, 25.f, 4.f, 0.f, -10.0f);
        RenderMeshIn2D(meshList[GEO_TQUIT], false, 20.f, 4.f, 0.f, -20.0f);
        break;
    }
    case 1:
    {
        RenderMeshIn2D(meshList[GEO_TBORDER], false, 33.f, 5.f, 0.f, -11.0f);
        RenderMeshIn2D(meshList[GEO_TMENU], false, 30.f, 4.f, 0.f, -10.0f);
        RenderMeshIn2D(meshList[GEO_TRESUME], false, 20.f, 4.f, 0.f, 1.0f);
        RenderMeshIn2D(meshList[GEO_TQUIT], false, 20.f, 4.f, 0.f, -20.0f);
        break;
    }
    case 2:
    {
        RenderMeshIn2D(meshList[GEO_TBORDER], false, 33.f, 5.f, 0.f, -21.0f);
        RenderMeshIn2D(meshList[GEO_TQUIT], false, 25.f, 4.f, 0.f, -20.0f);
        RenderMeshIn2D(meshList[GEO_TMENU], false, 25.f, 4.f, 0.f, -10.0f);
        RenderMeshIn2D(meshList[GEO_TRESUME], false, 20.f, 4.f, 0.f, 1.0f);
        break;
    }
    }
}

void SceneSP3::Exit()
{
    // Cleanup VBO
    for (int i = 0; i < NUM_GEOMETRY; ++i)
    {
        if (meshList[i])
            delete meshList[i];
    }
    while (particleList.size() > 0)
    {
        ParticleObject *particle = particleList.back();
        delete particle;
        particleList.pop_back();
    }
    while (seaList.size() > 0)
    {
        GameObject *go = seaList.back();
        delete go;
        seaList.pop_back();
    }
    while (projectileList.size() > 0)
    {
        GameObject *go = projectileList.back();
        delete go;
        projectileList.pop_back();
    }
    while (coralList.size() > 0)
    {
        GameObject *go = coralList.back();
        delete go;
        coralList.pop_back();
    }
    while (m_textList.size() > 0)
    {
        DamageText *to = m_textList.back();
        delete to;
        m_textList.pop_back();
    }

    glDeleteProgram(m_programID);
    glDeleteProgram(m_gPassShaderID);
    glDeleteVertexArrays(1, &m_vertexArrayID);
}
