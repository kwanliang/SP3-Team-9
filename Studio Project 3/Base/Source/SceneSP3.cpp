#include "SceneSP3.h"
#include "GL\glew.h"

#include <sstream>
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"

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

    //lights[0].type = Light::LIGHT_DIRECTIONAL;
    //lights[0].position.Set(0, 100, 50);
    //lights[0].color.Set(1, 1, 1);
    //lights[0].power = 3.0f;
    //lights[0].kC = 1.f;
    //lights[0].kL = 0.01f;
    //lights[0].kQ = 0.001f;
    //lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
    //lights[0].cosInner = cos(Math::DegreeToRadian(30));
    //lights[0].exponent = 3.f;

    //lights[1].type = Light::LIGHT_DIRECTIONAL;
    //lights[1].position.Set(0, 100, 50);
    //lights[1].color.Set(1, 1, 1);
    //lights[1].power = 1.0f;
    //lights[1].kC = 1.f;
    //lights[1].kL = 0.01f;
    //lights[1].kQ = 0.001;
    //lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
    //lights[1].cosInner = cos(Math::DegreeToRadian(30));
    //lights[1].exponent = 3.f;

    //lights[2].type = Light::LIGHT_SPOT;
    //lights[2].position.Set(-80.f, 100.f, 0.f);
    //lights[2].color.Set(1, 1, 1);
    //lights[2].power = 30.f;
    //lights[2].kC = 1.f;
    //lights[2].kL = 0.01f;
    //lights[2].kQ = 0.001f;
    //lights[2].spotDirection.Set(0.f, 1.f, 0.f);
    //lights[2].cosCutoff = cos(Math::DegreeToRadian(120));
    //lights[2].cosInner = cos(Math::DegreeToRadian(0));
    //lights[2].exponent = 3.f;

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
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1,1,1),1.f );
    meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");//, 1000, 1000, 1000);
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateQuad("minimap", Color(0, 0, 0), 2);
	meshList[GEO_CROSSHAIR]->textureID = LoadTGA("Image//crosshair.tga");
    meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
    meshList[GEO_TEXT]->textureID = LoadTGA("Image//sfont.tga");
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

    meshList[GEO_SKYPLANE] = MeshBuilder::GenerateSkyPlane("skyplane", Color(1, 1, 1), 128, 200.0f, 2000.0f, 1.0f, 1.0f);
    meshList[GEO_SKYPLANE]->textureArray[0] = LoadTGA("Image//sky.tga");
    meshList[GEO_SKYPLANE]->textureArray[1] = LoadTGA("Image//night.tga");

 //   meshList[GEO_TERRAIN0] = MeshBuilder::GenerateTerrain("terrain", "Image//Area0.raw", m_heightMap[0]);

	//meshList[GEO_TERRAIN2] = MeshBuilder::GenerateTerrain("terrain", "Image//Area02.raw", m_heightMap[2]);
	//meshList[GEO_TERRAIN3] = MeshBuilder::GenerateTerrain("terrain", "Image//Area03.raw", m_heightMap[3]);
	//meshList[GEO_TERRAIN4] = MeshBuilder::GenerateTerrain("terrain", "Image//Area04.raw", m_heightMap[4]);
    // meshList[GEO_TERRAIN]->textureArray[0] = LoadTGA("Image//rock.tga");
	//meshList[GEO_TERRAIN1] = MeshBuilder::GenerateTerrain("terrain", "Image//Area01.raw", m_heightMap[1]);
	//meshList[GEO_TERRAIN1]->textureArray[0] = LoadTGA("Image//rock.tga");
	//meshList[GEO_TERRAIN2] = MeshBuilder::GenerateTerrain("terrain", "Image//Area02.raw", m_heightMap[2]);
	//meshList[GEO_TERRAIN2]->textureArray[0] = LoadTGA("Image//rock.tga");
	//meshList[GEO_TERRAIN3] = MeshBuilder::GenerateTerrain("terrain", "Image//Area03.raw", m_heightMap[3]);
	//meshList[GEO_TERRAIN3]->textureArray[0] = LoadTGA("Image//rock.tga");
	//meshList[GEO_TERRAIN4] = MeshBuilder::GenerateTerrain("terrain", "Image//Area04.raw", m_heightMap[4]);
	//meshList[GEO_TERRAIN4]->textureArray[0] = LoadTGA("Image//rock.tga");

    //meshList[GEO_TERRAIN]->textureArray[1] = LoadTGA("Image//ForestMurky.tga");

	meshList[GEO_FISHMODEL] = MeshBuilder::GenerateOBJ("fishModel", "Models//OBJ//rcfish.obj");
	meshList[GEO_FISHMODEL]->textureArray[0] = LoadTGA("Image//RCfish.tga");
	meshList[GEO_FISHTAIL] = MeshBuilder::GenerateOBJ("fishModel", "Models//OBJ//rctail.obj");

    meshList[GEO_MINNOW] = MeshBuilder::GenerateOBJ("minnow", "Models//OBJ//minnow.obj");
	meshList[GEO_MINNOW]->textureArray[0] = LoadTGA("Image//minnow.tga");

    meshList[GEO_BALL] = MeshBuilder::GenerateSphere("ball", Color(0, 0, 0), 16, 16, 1.f);
    meshList[GEO_BALL2] = MeshBuilder::GenerateSphere("ball", Color(1, 0, 0), 16, 16, 1.f);

    // Object
    //meshList[OBJ_NAME] = MeshBuilder::GenerateOBJ("", "OBJ//.obj");
    //meshList[OBJ_NAME]->textureArray[0] = LoadTGA("Image//.tga");

    // Particles
    meshList[GEO_BUBBLE] = MeshBuilder::GenerateQuad("bubble", Color(1, 0, 0), 1.f);
    meshList[GEO_BUBBLE]->textureArray[0] = LoadTGA("Image//bubble.tga");

    meshList[GEO_VACUUM] = MeshBuilder::GenerateQuad("vacuum", Color(1, 0, 0), 1.f);
    meshList[GEO_VACUUM]->textureArray[0] = LoadTGA("Image//vacuum.tga");

    // HUD
    meshList[GEO_HUD_HEALTHBAR] = MeshBuilder::GenerateQuad("health bar hud", Color(1, 0, 0), 2.f);
    meshList[GEO_HUD_HEALTHBAR]->textureID = LoadTGA("Image//healthbar.tga");
    meshList[GEO_HUD_BOSSHEALTH] = MeshBuilder::GenerateQuad("boss health hud", Color(1, 0, 0), 2.f);
    meshList[GEO_HUD_BOSSHEALTH]->textureID = LoadTGA("Image//bosshealth.tga");

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

    // Shadow
    //meshList[GEO_LIGHT_DEPTH_QUAD] = MeshBuilder::GenerateQuad("Shadow Test", 1, 1);
    //meshList[GEO_LIGHT_DEPTH_QUAD]->textureArray[0] = m_lightDepthFBO.GetTexture();

    // Sprite
    //meshList[SPRITE_NAME] = MeshBuilder::GenerateSpriteAnimation("", 1, 5);
    //meshList[SPRITE_NAME]->textureArray[0] = LoadTGA("");

    //SPRITENAME = dynamic_cast<SpriteAnimation*>(meshList[SPRITE_NAME]);


	//SharedData::GetInstance()->SD_CurrentArea = 0;

    // Hitbox generation
	playerpos = Vector3(0, 0, 0);
	player_box = hitbox2::generatehitbox(playerpos, 10, 10, 10);
	fish_tailrot = 0;
	fish_tailmax = false;
	m_spCount = 0;
    //if (SPRITENAME)
    //{
    //    SPRITENAME->m_anim = new Animation();
    //    SPRITENAME->m_anim->Set(0, 4, 0, 1.f, true);
    //    SPRITENAME->SpritePos = Vector3(-80.f, 15.f + 350.f * ReadHeightMap(m_heightMap, -80.f / 3000.f, 0.f / 3000.f), 0.f);
    //    SPRITENAME->m_anim->moveable = true;
    //}

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
    //Mtx44 perspective;
    //perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
    //perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
    // projectionStack.LoadMatrix(perspective);

    rotateSky = 0.f;
    fogThickness = 0.f;
    blendFactor = 0.f;

    bLightEnabled = true;

    m_particleCount = 0;
    m_gravity.Set(0, -9.8f, 0);

	//init Minnow for test
	//Minnow *fo = FetchFO();
	//fo->active = true;
	//fo->objectType = GameObject::SEACREATURE;
	//fo->seaType = SeaCreature::MINNOW;
	//fo->state = Minnow::FLOCK;
	//fo->scale.Set(1, 1, 5);
	//fo->pos.Set(0,600,0);
	//fo->vel.Set(0,0,0);
	//fo->setHealth(50);
	
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
    m_goList.push_back(skipper);
    SharedData::GetInstance()->SD_IsImmobile = false;

    //skipper->collision = hitbox2::generatehitbox(minnowLeader->pos, 10, 10, 10);

 //   minnowLeader = new Minnow();
 //   minnowLeader->active = true;
 //   minnowLeader->objectType = GameObject::SEACREATURE;
 //   minnowLeader->seaType = SeaCreature::MINNOW;
 //   minnowLeader->state = Minnow::FLOCK;
 //   minnowLeader->setHealth(50);
 //   minnowLeader->setisLeader(true);
 //   minnowLeader->scale.Set(1, 1, 1);
 //   minnowLeader->pos.Set(Math::RandFloatMinMax(-100, 100), Math::RandFloatMinMax(400, 600), Math::RandFloatMinMax(-100, 100));
 //   minnowLeader->vel.Set(Math::RandFloatMinMax(-10, 10), 0, Math::RandFloatMinMax(-10, 10));
 //   minnowLeader->collision = hitbox2::generatehitbox(minnowLeader->pos, 10, 10, 10);
 //   g_MinnowLeaderCount++;
 //   m_goList.push_back(minnowLeader);

 //   for (int i = 0; i < 10; i++)
 //   {
 //       Minnow *fo = FetchMinnow();
 //       fo->active = true;
 //       fo->objectType = GameObject::SEACREATURE;
 //       fo->seaType = SeaCreature::MINNOW;
 //       fo->state = Minnow::FLOCK;
 //       fo->setisLeader(false);
 //       fo->scale.Set(1, 1, 1);
 //       fo->pos.Set(Math::RandFloatMinMax(-200, -100), Math::RandFloatMinMax(400, 600), Math::RandFloatMinMax(-100, 100));
 //       fo->vel.Set(Math::RandFloatMinMax(-10, 10), 0, Math::RandFloatMinMax(-10, 10));
 //       fo->collision = hitbox2::generatehitbox(fo->pos, 10, 10, 10);
 //       fo->setHealth(10);
 //       g_MinnowCount++;
 //   }

    DeathSelect = DEATHSELECT::RESPAWN;
}

Minnow* SceneSP3::FetchMinnow()
{
    for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
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
        m_goList.push_back(go);
    }
    Minnow *go = (Minnow *)m_goList.back();
    go->active = true;
    return go;
}

Pufferfish* SceneSP3::FetchPuffer()
{
    for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
    {
        Pufferfish *go = (Pufferfish *)*it;
        if (!go->active)
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
        m_goList.push_back(go);
    }
    Pufferfish *go = (Pufferfish *)m_goList.back();
    go->active = true;
    return go;
}

Fcrab* SceneSP3::FetchFCrab()
{
    for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
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
        m_goList.push_back(go);
    }
    Fcrab *go = (Fcrab *)m_goList.back();
    go->active = true;
    return go;
}

Cuttlefish* SceneSP3::FetchCuttle()
{
    for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
    {
        Cuttlefish *go = (Cuttlefish *)*it;
        if (!go->active)
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
        m_goList.push_back(go);
    }
    Cuttlefish *go = (Cuttlefish *)m_goList.back();
    go->active = true;
    return go;
}

Chimera* SceneSP3::FetchChimera()
{
    for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
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
        m_goList.push_back(go);
    }
    Chimera *go = (Chimera *)m_goList.back();
    go->active = true;
    return go;
}

//Drone* SceneSP3::FetchDrone()
//{
//    for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
//    {
//        Drone *go = (Drone *)*it;
//        if (!go->active)
//        {
//            go->objectType = GameObject::SEACREATURE;
//            go->seaType = SeaCreature::DRONE;
//            go->active = true;
//            return go;
//        }
//    }
//    for (unsigned i = 0; i < 10; ++i)
//    {
//        Drone *go = new Drone();
//        go->objectType = GameObject::SEACREATURE;
//        go->seaType = SeaCreature::DRONE;
//        m_goList.push_back(go);
//    }
//    Drone *go = (Drone *)m_goList.back();
//    go->active = true;
//    return go;
//}

Projectile* SceneSP3::FetchPO()
{
    for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
    {
        Projectile *go = (Projectile *)*it;
        if (!go->active)
        {
            go->active = true;
            //++m_objectCount;
            return go;
        }
    }
    for (unsigned i = 0; i < 10; ++i)
    {
        Projectile *go = new Projectile();
        go->objectType = GameObject::PROJECTILE;
        m_goList.push_back(go);
    }
    Projectile *go = (Projectile *)m_goList.back();
    go->active = true;
    //++m_objectCount;
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
            //++m_objectCount;
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
    //++m_objectCount;
    return go;
}

void SceneSP3::RenderTO(DamageText *to)
{  
    to->setScaleText(to->getScaleText() + Vector3(1, 1, 1));
    modelStack.PushMatrix();
    modelStack.Translate(to->getLastHitPos().x, to->getLastHitPos().y, to->getLastHitPos().z);
    modelStack.Rotate(LookAtPlayer(playerpos, to->getLastHitPos()), 0, 1, 0);
    modelStack.Scale(to->getScaleText().x, to->getScaleText().y, to->getScaleText().z);
    std::ostringstream ss;
    ss << to->getLastDamage();
    RenderText(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1));
    modelStack.PopMatrix();
    if (to->getScaleText().x > TextScaleValue)
    {
        to->setScaleText(Vector3(0, 0, 0));
        to->setActive(false);
    }
}

void SceneSP3::UpdateLoop(double dt)
{
    for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
    {
        GameObject *go = (GameObject *)*it;
        if (go->active && go->inRange)
        {
            // SEA CREATURE
            if (go->objectType == GameObject::SEACREATURE)
            {
                SeaCreature *sc = (SeaCreature *)*it;

                if (sc->getHealth() <= 0)
                    sc->active = false;

                // MINNOW
                if (sc->seaType == SeaCreature::MINNOW)
                {
                    Minnow *fo = (Minnow *)*it;

                    if (fo->getisLeader())
                        fo->pos += fo->vel * dt * 2;
                    else if (fo->state == Minnow::FLOCK)
                        fo->pos += fo->vel * dt;
                    else
                        fo->pos += fo->vel * dt * 10;

                    hitbox2::updatehitbox(fo->collision, fo->pos);

                    Vector3 tempCentreOfMass(0, 0, 0);
                    Vector3 tempRepelVector(0, 0, 0);
                    Vector3 tempForceVector(0, 0, 0);
                    Vector3 ahead(0, 0, 0);

                    if (!fo->vel.IsZero())
                        ahead = fo->pos + fo->vel.Normalized() * 5;

                    if (terraincollision(ahead, m_heightMap[SharedData::GetInstance()->SD_CurrentArea]))
                    {
                        fo->state = Minnow::COLLIDING;
                        fo->vel *= -5;
                        fo->vel = Vector3(fo->vel.x / cos(30.f), fo->vel.y / cos(30.f), fo->vel.z / cos(30.f));
                    }
                    else if (fo->state != Minnow::FLEE)
                    {
                        fo->state = Minnow::FLOCK;
                    }

                    for (std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
                    {
                        Minnow *other = (Minnow *)*it2;
                        if (other->active)
                        {
                            if (other->objectType == GameObject::SEACREATURE)
                            {
                                if (fo != other && other->seaType == SeaCreature::MINNOW &&other->getisLeader())
                                {
                                    if (fo->state == Minnow::FLOCK && !fo->getisLeader() && (other->pos - fo->pos).LengthSquared() < g_MinnowAttractRange * 2)
                                        fo->vel += fo->cohesion(other) + fo->alignment(tempForceVector);
                                    else if (fo->state == Minnow::FLEE && !fo->getisLeader())
                                        fo->vel += fo->seperation(tempCentreOfMass) * 2;
                                }
                                else if (other->active && fo != other && fo->seaType == SeaCreature::MINNOW && fo->state == Minnow::FLOCK)
                                {
                                    if ((fo->pos - other->pos).LengthSquared() < g_distFromSeperation)
                                    {
                                        tempRepelVector = other->pos - fo->pos;
                                        other->vel += other->seperation(tempRepelVector);
                                    }
                                }
                            }
                        }
                    }

                    // Cap velocity
                    if (go->vel.x > 20)
                        go->vel.x = 20;
                    if (go->vel.y > 20)
                        go->vel.y = 20;
                    if (go->vel.z > 20)
                        go->vel.z = 20;
                    if (go->vel.x < -20)
                        go->vel.x = -20;
                    if (go->vel.y < -20)
                        go->vel.y = -20;
                    if (go->vel.z < -20)
                        go->vel.z = -20;

                    if (fo->state == Minnow::FLEE && fo->getpanicTime() < 3.f)
                    {
                        fo->setpanicTime(fo->getpanicTime() + dt);
                    }

                    if (fo->state == Minnow::FLEE && fo->getpanicTime() >= 3.f)
                    {
                        fo->state = Minnow::FLOCK;
                        fo->setpanicTime(0.f);
                    }
                }
                // PUFFERFISH
                else if (sc->seaType == SeaCreature::PUFFER)
                {
                    Pufferfish *fo = (Pufferfish*)*it;

                    hitbox2::updatehitbox(fo->collision, fo->pos);
                    Vector3 displacement = playerpos - fo->pos;

                    switch (fo->pstate)
                    {
                    case Pufferfish::IDLE:
                    {
                        if (collision(fo->collision, player_box))
                        {
                            fo->setMoveCoolDown(0.0);
                            fo->vel *= -2;

                            if (fishVel.Length() < 3)
                                fishVel = Vector3(5, 5, 5);

                            fishVel *= -2.f;
                            walkCam.Move(fishVel * (float)dt);
                            playerpos = walkCam.GetPos() + Vector3(0, 80, 0);

                            skipper->setHealth(skipper->getHealth() - 10);

                            DamageText* text = FetchTO();
                            text->setActive(true);
                            text->setLastHitPos(playerpos + walkCam.GetDir().Normalized() * 5 + Vector3(0, 10, 0));
                            text->setLastDamage(10);
                            text->setScaleText(Vector3(0, 0, 0));

                            fo->scale = Vector3(10, 10, 10);
                            fo->pstate = Pufferfish::ENRAGED;
                        }

                        fo->pos += fo->vel * dt;

                        break;
                    }
                    case Pufferfish::ENRAGED:
                    {
                        if (displacement.LengthSquared() > 400 * 400)
                        {
                            fo->vel *= -1.f;
                            fo->scale = Vector3(5, 5, 5);
                            fo->pstate = Pufferfish::IDLE;
                        }

                        if (collision(fo->collision, player_box) && skipper->getTimerReceieveDamage() > 1.0)
                        {
                            skipper->setTimerReceieveDamage(0.0);

                            fo->setMoveCoolDown(0.0);
                            fo->vel *= -2;

                            if (fishVel.Length() < 3)
                                fishVel = Vector3(5, 5, 5);

                            fishVel *= -2.f;
                            walkCam.Move(fishVel * (float)dt);
                            playerpos = walkCam.GetPos() + Vector3(0, 80, 0);

                            skipper->setHealth(skipper->getHealth() - 20);

                            DamageText* text = FetchTO();
                            text->setActive(true);
                            text->setLastHitPos(playerpos + walkCam.GetDir().Normalized() * 5 + Vector3(0, 10, 0));
                            text->setLastDamage(20);
                            text->setScaleText(Vector3(0, 0, 0));
                        }

                        if (fo->getMoveCoolDown() > 0.5)
                        {
                            fo->vel += displacement.Normalized();
                        }

                        if (fo->vel.x > 50)
                            fo->vel.x = 50;
                        if (fo->vel.y > 50)
                            fo->vel.y = 50;
                        if (fo->vel.z > 50)
                            fo->vel.z = 50;
                        if (fo->vel.x < -50)
                            fo->vel.x = -50;
                        if (fo->vel.y < -50)
                            fo->vel.y = -50;
                        if (fo->vel.z < -50)
                            fo->vel.z = -50;

                        fo->setMoveCoolDown(fo->getMoveCoolDown() + dt);
                        fo->pos += fo->vel * dt;

                        break;
                    }
                    }

                    // terrain collision
                    if (terraincollision(fo->collision, m_heightMap[SharedData::GetInstance()->SD_CurrentArea]))
                    {
                        fo->vel *= -5.f;
                        fo->pos.y += 4;
                    }

                    // height limit
                    if (fo->pos.y > 1000)
                    {
                        fo->pos.y -= 1;
                        fo->vel.y = -fo->vel.y;
                    }
                }
                else if (sc->seaType == SeaCreature::FCRAB)
                {
                    Fcrab* c = (Fcrab*)*it;

                    c->pos += c->vel*dt;
                    float h = 350.f * ReadHeightMap(m_heightMap[2], c->pos.x / 3000.f, c->pos.z / 3000.f) + 4;//get height

                    Vector3 displacement = SharedData::GetInstance()->SD_PlayerPos - c->pos;

                    if (c->pos.y > 300)// reset crabs who move out of range
                    {
                        float x = Math::RandFloatMinMax(-300, 0);
                        float z = Math::RandFloatMinMax(-1000, -800);
                        float y = 350.f * ReadHeightMap(m_heightMap[2], x / 3000.f, z / 3000.f) + 4;
                        c->pos.Set(x, y, z);

                    }

                    switch (c->FCstate)
                    {
                    case Fcrab::IDLE:
                    {
                        float theta = 0;
                        if (displacement.LengthSquared() < 50 * 50)
                        {
                            c->pos.y += 1;
                            theta = Math::RadianToDegree(atan2(displacement.y, Vector3(displacement.x, 0, displacement.z).Length()));
                            c->vel = displacement.Normalized() * 30;
                            c->FCstate = Fcrab::ATTACKING;
                        }

                        if (c->pos.y < h)
                            c->pos.y += (float)dt * 10;
                        else if (c->pos.y > h)
                            c->pos.y -= (float)dt * 10;
                        break;
                    }
                    case Fcrab::ATTACKING:
                    {
                        c->vel.y -= 9.8f * (float)dt;

                        if (collision(c->aabb, player_box) && skipper->getTimerReceieveDamage() > 1.0)
                        {
                            skipper->setTimerReceieveDamage(0.0);

                            fishVel = -c->vel;

                            skipper->setHealth(skipper->getHealth() - 20);

                            DamageText* text = FetchTO();
                            text->setActive(true);
                            text->setLastHitPos(playerpos + walkCam.GetDir().Normalized() * 5 + Vector3(0, 10, 0));
                            text->setLastDamage(20);
                            text->setScaleText(Vector3(0, 0, 0));
                        }

                        if (c->pos.y <= h)
                        {
                            c->vel.Set(Math::RandFloatMinMax(0, 4), 0, Math::RandFloatMinMax(3, 6));

                            c->pos.y = h;
                            c->FCstate = Fcrab::IDLE;
                            break;
                        }
                    }
                    }

                    c->UpdateFcrab(dt);//run update for fcrabs
                }
                else if (sc->seaType == SeaCreature::CUTTLE)
                {
                    Cuttlefish *cf = (Cuttlefish*)*it;

                    if (terraincollision(cf->collision, m_heightMap[3]))//collision with terrain
                    {
                        cf->pos.y += 2;
                        cf->vel = -cf->vel;
                    }

                    if (collision(cf->collision, player_box) && skipper->getTimerReceieveDamage() > 1.0)
                    {
                        skipper->setTimerReceieveDamage(0.0);

                        fishVel = -cf->vel;

                        skipper->setHealth(skipper->getHealth() - 20);

                        DamageText* text = FetchTO();
                        text->setActive(true);
                        text->setLastHitPos(playerpos + walkCam.GetDir().Normalized() * 5 + Vector3(0, 10, 0));
                        text->setLastDamage(20);
                        text->setScaleText(Vector3(0, 0, 0));
                    }

                    cf->UpdateCuttle(dt);
                }
                else if (sc->seaType == SeaCreature::CHIMERA)
                {
                    Chimera * c = (Chimera*)*it;

                    if (collision(c->aabb, player_box))
                        c->chstate = Chimera::FLEEING;

                    if (collision(c->aabb, player_box) && skipper->getTimerReceieveDamage() > 1.0)
                    {
                        c->chstate = Chimera::FLEEING;

                        skipper->setTimerReceieveDamage(0.0);

                        fishVel = -c->vel;

                        skipper->setHealth(skipper->getHealth() - 20);

                        DamageText* text = FetchTO();
                        text->setActive(true);
                        text->setLastHitPos(playerpos + walkCam.GetDir().Normalized() * 5 + Vector3(0, 10, 0));
                        text->setLastDamage(20);
                        text->setScaleText(Vector3(0, 0, 0));
                    }

                    c->UpdateChimera(dt);
                }
            }
            // PROJECTILE
            else if (go->objectType == GameObject::PROJECTILE)
            {
                Projectile *po = (Projectile *)*it;
                po->pos += po->vel * (float)dt;

                po->setLifetime(po->getLifetime() - dt);

                if (po->getLifetime() <= 0.0)
                {
                    po->active = false;
                }

                // BULLET
                if (po->projectileType == Projectile::BULLET)
                {
                    for (std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
                    {
                        GameObject* other = (GameObject *)*it2;
                        if (other->active)
                        {
                            // Collision check of BULLET on SEA CREATURES
                            if (other->objectType == GameObject::SEACREATURE && go != other)
                            {
                                SeaCreature *sc = (SeaCreature *)*it2;
                                if (sc->seaType == SeaCreature::MINNOW)
                                {
                                    Minnow *fo = (Minnow *)*it2;
                                    if (fo->state == Minnow::FLOCK && (fo->pos - po->pos).LengthSquared() < 5000)
                                    {
                                        fo->state = Minnow::FLEE;
                                        fo->vel.Set(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10));
                                    }
                                    if ((fo->pos - po->pos).LengthSquared() < po->scale.z + fo->scale.z)
                                    {
                                        po->active = false;
                                        fo->active = false;
                                        cout << "dead" << endl;
                                        g_MinnowCount--;
                                    }
                                }
                                else if (sc->seaType == SeaCreature::PUFFER)
                                {
                                    Pufferfish *fo = (Pufferfish *)*it2;

                                    Vector3 displacement = po->pos - fo->pos;
                                    if (displacement.LengthSquared() < fo->scale.z + fo->scale.z + 50)
                                    {
                                        fo->scale = Vector3(10, 10, 10);
                                        if (fo->pstate == Pufferfish::IDLE)
                                            fo->pstate = Pufferfish::ENRAGED;

                                        po->active = false;
                                        fo->setHealth(fo->getHealth() - skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));

                                        DamageText* text = FetchTO();
                                        text->setActive(true);
                                        text->setLastHitPos(po->pos);
                                        text->setLastDamage(skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));
                                        text->setScaleText(Vector3(0, 0, 0));
                                    }
                                }
                                else if (sc->seaType == SeaCreature::FCRAB)
                                {
                                    Fcrab *fo = (Fcrab *)*it2;

                                    Vector3 displacement = po->pos - fo->pos;
                                    if (displacement.LengthSquared() < fo->scale.z + fo->scale.z + 10)
                                    {
                                        if (fo->FCstate == Fcrab::IDLE)
                                            fo->FCstate = Fcrab::ATTACKING;

                                        po->active = false;
                                        fo->setHealth(fo->getHealth() - skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));

                                        DamageText* text = FetchTO();
                                        text->setActive(true);
                                        text->setLastHitPos(po->pos);
                                        text->setLastDamage(skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));
                                        text->setScaleText(Vector3(0, 0, 0));
                                    }
                                }
                                else if (sc->seaType == SeaCreature::CUTTLE)
                                {
                                    Cuttlefish *fo = (Cuttlefish *)*it2;

                                    Vector3 displacement = po->pos - fo->pos;
                                    if (displacement.LengthSquared() < fo->scale.z + fo->scale.z + 50)
                                    {
                                        if (fo->ctstate == Cuttlefish::IDLE)
                                            fo->ctstate = Cuttlefish::ATTACKING;

                                        po->active = false;
                                        fo->setHealth(fo->getHealth() - skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));

                                        DamageText* text = FetchTO();
                                        text->setActive(true);
                                        text->setLastHitPos(po->pos);
                                        text->setLastDamage(skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));
                                        text->setScaleText(Vector3(0, 0, 0));
                                    }
                                }
                                else if (sc->seaType == SeaCreature::CHIMERA)
                                {
                                    Chimera *fo = (Chimera *)*it2;

                                    Vector3 displacement = po->pos - fo->pos;
                                    if (displacement.LengthSquared() < fo->scale.z + fo->scale.z + 50)
                                    {
                                        if (fo->chstate == Chimera::IDLE)
                                            fo->chstate = Chimera::ATTACKING;

                                        po->active = false;
                                        fo->setHealth(fo->getHealth() - skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));

                                        DamageText* text = FetchTO();
                                        text->setActive(true);
                                        text->setLastHitPos(po->pos);
                                        text->setLastDamage(skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));
                                        text->setScaleText(Vector3(0, 0, 0));
                                    }
                                }

                            }
                            // Collision check of BULLET on BOSS
                            else if (other->objectType == GameObject::BOSS && go != other)
                            {
                                Boss* bo = (Boss*)*it2;
                                if (bo->bossType == Boss::GIANTSQUID)
                                {
                                    GiantSquid *squid = (GiantSquid *)*it2;
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
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                else if (po->projectileType == Projectile::INK)
                {
                    if ((po->pos - playerpos).LengthSquared() < (po->scale.x * po->scale.x) + (skipper->scale.x * skipper->scale.x))
                    {
                        po->active = false;

                        skipper->setHealth(skipper->getHealth() - 20);

                        DamageText* text = FetchTO();
                        text->setActive(true);
                        text->setLastHitPos(playerpos + walkCam.GetDir().Normalized() + Vector3(0, 10, 0));
                        text->setLastDamage(20);
                        text->setScaleText(Vector3(0, 0, 0));
                    }
                }
            }
        }
    }
}

void SceneSP3::UpdateCaptured(double dt)
{
	// Minnow loop
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			if (go->objectType == GameObject::CAPTURED)
			{
				SeaCreature *fo = (SeaCreature *)*it;
				
						fo->pos += fo->vel * dt;
					// Collision
					hitbox2::updatehitbox(fo->collision, fo->pos);

					Vector3 tempCentreOfMass(0, 0, 0);
					Vector3 tempRepelVector(0, 0, 0);
					Vector3 tempForceVector(0, 0, 0);

					if (terraincollision(fo->collision, m_heightMap[SharedData::GetInstance()->SD_CurrentArea]))
					{
						fo->vel *= -5;
					}

					for (std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
					{
						Minnow *other = (Minnow *)*it2;
						if (other->active)
						{
							if ((playerpos - other->pos).LengthSquared() < g_distFromSeperation && (other->seaType == SeaCreature::MINNOW ||  skipper->getTarget()->objectType == GameObject::PLAYER))
							{
								tempRepelVector = other->pos - playerpos;
								other->vel += other->seperation(tempRepelVector);
							}
						}
					}
					if (fo->seaType==SeaCreature::MINNOW || skipper->getTarget()->objectType == GameObject::PLAYER)
					{
						fo->vel += fo->cohesion(playerpos, walkCam.GetDir()) + fo->alignment(tempForceVector);

					}

					// Cap velocity
					if (go->vel.x > 20)
						go->vel.x = 20;
					if (go->vel.y > 20)
						go->vel.y = 20;
					if (go->vel.z > 20)
						go->vel.z = 20;
					if (go->vel.x < -20)
						go->vel.x = -20;
					if (go->vel.y < -20)
						go->vel.y = -20;
					if (go->vel.z < -20)
						go->vel.z = -20;
					if ((playerpos-go->pos).LengthSquared() > 100000)
					{
						go->pos.Set(playerpos.x, playerpos.y, playerpos.z);
					}
				
			}
		}
	}
}

void SceneSP3::SaveCaptured()
{
	SharedData::GetInstance()->SD_CapturedList.clear();
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			if (go->objectType == GameObject::CAPTURED)
			{
				SharedData::GetInstance()->SD_CapturedList.push_back(go);
			}
		}
	}
}

void SceneSP3::ReinitCaptured()
{
	for (std::vector<GameObject *>::iterator it = SharedData::GetInstance()->SD_CapturedList.begin(); it != SharedData::GetInstance()->SD_CapturedList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			if (go->objectType == GameObject::CAPTURED)
			{
				m_goList.push_back(go);
			}
		}
	}
}

void SceneSP3::UpdateTravel()
{
	//if (!SharedData::GetInstance()->SD_Travel)
	//return;

	if ( collision(m_travelzonedown, player_box.m_point[0]) || collision(m_travelzonedown, player_box.m_point[6]))//travel downward
	{

		if (static_cast<SharedData::AREA>(SharedData::GetInstance()->SD_CurrentArea) != SharedData::A_NIGHTMARETRENCH)
		{
			SaveCaptured();
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
			SaveCaptured();
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
    const float acceleration = 50.f;
    const float speedLimit = 50.f;

    if (Application::IsKeyPressed('1'))
        glEnable(GL_CULL_FACE);
    if (Application::IsKeyPressed('2'))
        glDisable(GL_CULL_FACE);
    if (Application::IsKeyPressed('3'))
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (Application::IsKeyPressed('4'))
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    if (Application::IsKeyPressed('I'))
        lights[0].position.z -= (float)(100.f * dt);
    if (Application::IsKeyPressed('K'))
        lights[0].position.z += (float)(100.f * dt);
    if (Application::IsKeyPressed('J'))
        lights[0].position.x -= (float)(100.f * dt);
    if (Application::IsKeyPressed('L'))
        lights[0].position.x += (float)(100.f * dt);
    if (Application::IsKeyPressed('O'))
        lights[0].position.y -= (float)(100.f * dt);
    if (Application::IsKeyPressed('P'))
        lights[0].position.y += (float)(100.f * dt);
    if (Application::IsKeyPressed('C'))
    {
        std::cout << playerpos << std::endl;
    }

    static bool bSPACEstate = false;
    if (!bSPACEstate && Application::IsKeyPressed(VK_SPACE))
    {
        bSPACEstate = true;
    }
    else if (bSPACEstate && !Application::IsKeyPressed(VK_SPACE))
    {
        Projectile *po = FetchPO();

        static const float focusLength = 300.f;

        static const float projectileSpeed = 150.f;

        Vector3 focusPoint = walkCam.GetPos() + Vector3(0, walkCam.yOffset, 0) + walkCam.GetDir() * (focusLength - walkCam.distance);

        po->objectType = GameObject::PROJECTILE;
        po->projectileType = Projectile::BULLET;
        po->active = true;
        po->scale.Set(1, 1, 1);
        po->pos = playerpos;

        //po->vel = (focusPoint - playerpos).Normalized();
        //po->vel += fishVel;
        po->setLifetime(3.);
        Vector3 bulletDirection = focusPoint - playerpos;
        bulletDirection.Normalize();
        po->vel = bulletDirection * projectileSpeed + fishVel;
        bSPACEstate = false;
    }

    //camera.Update(dt);
    fps = 1.0f / (float)dt;

    Vector3 temp = walkCam.GetPos();
    Vector3 right = walkCam.GetDir().Cross(walkCam.GetUp());
    right.Normalize();
    float movespeed = 1000;
    if (Application::IsKeyPressed('Z'))
    {
        val += 20.f * (float)dt;
        cout << val << endl;
    }
    if (Application::IsKeyPressed('X'))
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
        if (Application::IsKeyPressed('W'))
        {
            forceApplied += walkCam.GetDir() * acceleration;
        }
        if (Application::IsKeyPressed('S'))
        {
            forceApplied -= walkCam.GetDir() * acceleration;
        }

        if (Application::IsKeyPressed('A'))
        {
            forceApplied -= right * acceleration;
        }
        if (Application::IsKeyPressed('D'))
        {
            forceApplied += right * acceleration;
        }

        for (unsigned short i = 0; i < 256; ++i)
        {
            break; //Comment this break break statement to test keys.
            if (Application::IsKeyPressed(i))
                std::cout << i << std::endl;
        }

        //Correspondance:
        //Is held; Is pressed
        static bool boostKeyStatus[] = { false, false, false };

        boostKeyStatus[0] = Application::IsKeyPressed('1');
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

        if (Application::IsKeyPressed('H'))
            skipper->setHealth(0);
    }

    rotateSky += .05f;

 //   // Fog & Blending
	//if (blendFactor < 1.0f)
	//	blendFactor = 1.f;
	//else
 //       blendFactor = 0;

    //glUniform1f(m_parameters[U_TEXTURE_BLEND_FACTOR], blendFactor);

    //    fogThickness += (1.f / 100.f);

    //glUniform1f(m_parameters[U_FOG_THICKNESS], fogThickness);
    //fogThickness = 0;

    // Sprite
    //SPRITENAME = dynamic_cast<SpriteAnimation*>(meshList[SPRITE_NAME]);

    //SPRITENAME->Update(dt);
    //SPRITENAME->m_anim->animActive = true;

    for (auto it : m_goList)
    {
        GameObject* go = (GameObject*)it;
        if (go->active)
        {
        if ((go->pos - playerpos).LengthSquared() < 1000000)
            go->inRange = true;
        else
            go->inRange = false;
            }
    }

    UpdateParticles(dt);
	UpdateTravel();
    UpdateLoop(dt);
	UpdateCaptured(dt);
	UpdateSquadFire(dt);
    UpdateSpawner(dt);

	if (Application::IsKeyPressed('M'))
	{
		for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			SeaCreature *go = (SeaCreature *)*it;
			if (go->active)
			{
				if (go->objectType == GameObject::SEACREATURE)
				{
					go->pos.Set(Capture::Vacuum(*go, playerpos, Capture::rangeCheckXZ(walkCam, *go, playerpos)).x,
								Capture::Vacuum(*go, playerpos, Capture::rangeCheckXZ(walkCam, *go, playerpos)).y,
								Capture::Vacuum(*go, playerpos, Capture::rangeCheckXZ(walkCam, *go, playerpos)).z);
					go->objectType = Capture::AddSquad(*go, playerpos, Capture::rangeCheckXZ(walkCam, *go, playerpos));
				}
			}
		}	
	}

	if (Application::IsKeyPressed('N'))
	{
		skipper->setTarget(skipper);
	}



//std::cout << m_spCount << std::endl;
}

void SceneSP3::UpdateSpawner(double dt)
{
    // Spawner
    MinnowLeaderSpawner.CheckCount(g_MinnowLeaderCount, g_MaxMinnowLeader);

    if (MinnowLeaderSpawner.getIsSpawn())
    {
        Vector3 tv1(Math::RandFloatMinMax(-1000.f, 1000.f), Math::RandFloatMinMax(0, 1000.f), Math::RandFloatMinMax(-1000.f, 1000.f));
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
        Vector3 tv2(Math::RandFloatMinMax(-1000.f, 1000.f), Math::RandFloatMinMax(0, 1000.f), Math::RandFloatMinMax(-1000.f, 1000.f));
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
            Vector3 tv(Math::RandFloatMinMax(-1000.f, 1000.f), Math::RandFloatMinMax(0, 1000.f), Math::RandFloatMinMax(-1000.f, 1000.f));
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
            Vector3 tv(Math::RandFloatMinMax(-1000.f, 1000.f), Math::RandFloatMinMax(0, 1000.f), Math::RandFloatMinMax(-1000.f, 1000.f));
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
            Vector3 tv(Math::RandFloatMinMax(-1000.f, 1000.f), Math::RandFloatMinMax(0, 1000.f), Math::RandFloatMinMax(-1000.f, 1000.f));
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
            Vector3 tv(Math::RandFloatMinMax(0.f, 1000.f), Math::RandFloatMinMax(0, 1000.f), Math::RandFloatMinMax(0.f, 1000.f));
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

        if (isopod->getSpawnIsopodDrone())
        {
            IsopodDroneSpawner.CheckCount(g_IsopodDroneCount, g_MaxIsopodDrone);

            if (IsopodDroneSpawner.getIsSpawn())
            {
                //Drone* d = FetchDrone();
                //d->active = true;
                //d->objectType = GameObject::SEACREATURE;
                //d->seaType = SeaCreature::DRONE;
                //d->scale.Set(SCALE, SCALE, SCALE);
                //d->pos.Set(isopod->pos.x, isopod->pos.y, isopod->pos.z);
                //d->vel.Set(0, 0, 0);
                //d->aabb = 
                //d->setHealth();

                g_IsopodDroneCount++;
            }
        }

        break;
    }
    }
}

void SceneSP3::UpdateSquadFire(double dt)
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			if (go->objectType == GameObject::CAPTURED)
			{
				SeaCreature *other = (SeaCreature *)*it;
				
				if (skipper->getTarget()->objectType != GameObject::PLAYER)
				{
					
					if (other->getDebounceTimer() > 3 && other->seaType == SeaCreature::MINNOW)
					{
						for (std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
						{
							GameObject *go2 = (GameObject *)*it2;
							if (go2 == skipper->getTarget())
							{
								Projectile *po = FetchPO();
								po->setLifetime(10);
								po->objectType = GameObject::PROJECTILE;
								po->projectileType = Projectile::BULLET;
								po->active = true;
								po->scale.Set(1, 1, 1);
								po->pos.Set(other->pos.x, other->pos.y, other->pos.z);
								Vector3 view = (skipper->getTarget()->pos - other->pos).Normalized();
								if (go2->objectType == GameObject::BOSS)
								{
									Boss *another = (Boss *)*it2;
									view = (another->collision.m_position - other->pos).Normalized();
								}

								po->vel.Set(view.x / 10, view.y / 10, view.z / 10);
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
						if (skipper->getTarget()->objectType == GameObject::BOSS)
						{
							for (std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
							{
								GameObject* another = (GameObject*)*it2;
								if (another->objectType == GameObject::BOSS)
								{
									Boss* another = (Boss*)*it2;
									if (!(another->collision.m_position - other->pos).IsZero())
									{
										Vector3 view = (another->collision.m_position - other->pos).Normalized();
										other->vel.Set(view.x * 50, view.y * 50, view.z / 50);
									}
								}
							}
						}
						else
						{
							if (!(skipper->getTarget()->pos - other->pos).IsZero())
							{
								Vector3 view = (skipper->getTarget()->pos - other->pos).Normalized();
								other->vel.Set(view.x * 10, view.y * 10, view.z / 10);
							}
						}

						if (other->getDebounceTimer() > 1.5)//&& (collision(another->collision, go->pos)))
						{
							if (skipper->getTarget()->objectType == GameObject::BOSS)
							{
								for (std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
								{
									Boss* another = (Boss*)*it2;
									if (another->objectType == GameObject::BOSS && collision(another->collision, other->pos))
									{
										another->setHealth(another->getHealth() - skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));

										DamageText* text = FetchTO();
										text->setActive(true);
										text->setLastHitPos(go->pos);
										text->setLastDamage(skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));
										text->setScaleText(Vector3(0, 0, 0));
										cout << skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()) << endl;
										other->setDebounceTimer(0);
									}
								}
							}
							else if (other->pos == skipper->getTarget()->pos)
							{
								/*for (std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
								{
									GameObject* another = (GameObject*)*it2;
									if (skipper->getTarget() == (GameObject)it2 && collision(another->collision, other->pos))
									{
										SeaCreature* another = (SeaCreature*)*it2;

										another->setHealth(another->getHealth() - skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));

										DamageText* text = FetchTO();
										text->setActive(true);
										text->setLastHitPos(go->pos);
										text->setLastDamage(skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()));
										text->setScaleText(Vector3(0, 0, 0));
										cout << skipper->randomDamage(skipper->getDamage(), skipper->getBaseDamage()) << endl;
										other->setDebounceTimer(0);
									}
								}*/
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
	}
}

float SceneSP3::LookAtPlayer(Vector3 playerpos, Vector3 otherpos)
{
    Vector3 initView(0, 0, 1);
    Vector3 wantView(playerpos - otherpos);
    Vector3 normal(0, 1, 0);

    if (otherpos != (0, 0, 0))
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
        characterSpacing.SetToTranslation(i * 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
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

    for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
    {
        GameObject* go = (GameObject*)*it;
        if (go->objectType != GameObject::SEACREATURE)
            continue;
        else
        {
            SeaCreature* sc = (SeaCreature*)*it;
            if (sc->active && sc->getHealth() <= 50)
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
	if (m_spCount < 30)
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

                for (auto it : m_goList)
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
	float range = 120;
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
        if (!particle->active && PARTICLEOBJECT_TYPE::P_VACUUM && !PARTICLEOBJECT_TYPE::P_PARTICLE)
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
                //modelStack.Rotate(particle->rotation, 0, 1, 0);
                modelStack.Scale(0.2, 0.2, 0.2);
                RenderMesh(meshList[GEO_CUBE], false);
                modelStack.PopMatrix();
                break;

            }
        }
    }
}

void SceneSP3::RenderSquad(SeaCreature *go)
{
	if (go->objectType == GameObject::CAPTURED)
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
			RenderMesh(meshList[GEO_MINNOW], false);
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
			RenderMesh(meshList[GEO_FCRABBODY], false);

			modelStack.PushMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(0.15, 0, -0.17);
			modelStack.Rotate(c->Crableg[0].rotate, 1, 0, 0);
			RenderMesh(meshList[GEO_FCRABLEG], false);//leg1
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-0.15, 0, -0.17);
			modelStack.Rotate(c->Crableg[1].rotate, 1, 0, 0);
			RenderMesh(meshList[GEO_FCRABLEG], false);//leg2
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-0.45, 0, -0.17);
			modelStack.Rotate(c->Crableg[2].rotate, 1, 0, 0);
			RenderMesh(meshList[GEO_FCRABLEG], false);//leg3
			modelStack.PopMatrix();
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Rotate(180, 0, 1, 0);

			modelStack.PushMatrix();
			modelStack.Translate(-0.15, 0, -0.17);
			modelStack.Rotate(c->Crableg[3].rotate, 1, 0, 0);
			RenderMesh(meshList[GEO_FCRABLEG], false);//leg4
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(0.15, 0, -0.17);
			modelStack.Rotate(c->Crableg[4].rotate, 1, 0, 0);
			RenderMesh(meshList[GEO_FCRABLEG], false);//leg5
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(0.45, 0, -0.17);
			modelStack.Rotate(c->Crableg[5].rotate, 1, 0, 0);
			RenderMesh(meshList[GEO_FCRABLEG], false);//leg6
			modelStack.PopMatrix();

			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(0.17, 0, -0.35);
			modelStack.Rotate(-55, 0, 1, 0);
			RenderMesh(meshList[GEO_FCRABCLAW], false);
			modelStack.PopMatrix();

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
			RenderMesh(meshList[GEO_CHIMERA_BODY], false);

			modelStack.PushMatrix();
			modelStack.Translate(0, -0.05, 0.18);
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
			RenderMesh(meshList[GEO_CUTTLE], true);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			modelStack.PopMatrix();
			break;
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
    }
}

void SceneSP3::RenderPO(Projectile *po)
{
    modelStack.PushMatrix();
    modelStack.Translate(po->pos.x, po->pos.y, po->pos.z);
    modelStack.Scale(po->scale.x, po->scale.y, po->scale.z);
    RenderMesh(meshList[GEO_BALL], false);
    modelStack.PopMatrix();
}

void SceneSP3::RenderLoop()
{
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
        static bool b_DS_UPState = false;
        if (Application::IsKeyPressed('W') && !b_DS_UPState/* ||
            Application::IsKeyPressed(VK_UP) && !b_DS_UPState*/)
        {
            b_DS_UPState = true;
        }
        else if (!Application::IsKeyPressed('W') && b_DS_UPState/* ||
            !Application::IsKeyPressed(VK_UP) && b_DS_UPState*/)
        {

            if (DeathSelect != DEATHSELECT::RESPAWN)
            {
                int currentDeathSelect = static_cast<int>(DeathSelect);
                currentDeathSelect--;
                DeathSelect = static_cast<DEATHSELECT>(currentDeathSelect);
            }
            b_DS_UPState = false;
        }

        static bool b_DS_DOWNState = false;

        if (Application::IsKeyPressed('S') && !b_DS_DOWNState/* ||
            Application::IsKeyPressed(VK_DOWN) && !b_DS_DOWNState*/)
        {
            b_DS_DOWNState = true;
        }
        else if (!Application::IsKeyPressed('S') && b_DS_DOWNState/* ||
            !Application::IsKeyPressed(VK_DOWN) && b_DS_DOWNState*/)
        {
            if (DeathSelect != DEATHSELECT::QUIT)
            {
                int currentDeathSelect = static_cast<int>(DeathSelect);
                currentDeathSelect++;
                DeathSelect = static_cast<DEATHSELECT>(currentDeathSelect);
            }
            b_DS_DOWNState = false;
        }

        glBlendFunc(GL_ONE, GL_ONE);
        RenderMeshIn2D(meshList[GEO_TLAYER], false, 35.f, 30.f);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
        RenderMeshIn2D(meshList[GEO_TDIED], false, 30.f, 5.f, 0.f, 20.0f);

        static bool b_DSENTERState = false;
        switch (DeathSelect)
        {
        case RESPAWN:
        {
            RenderMeshIn2D(meshList[GEO_TBORDER], false, 33.f, 5.f, 0.f, 1.0f);
            RenderMeshIn2D(meshList[GEO_TRESPAWN], false, 30.f, 5.f, 0.f, 0.0f);
            RenderMeshIn2D(meshList[GEO_TMENU], false, 25.f, 5.f, 0.f, -10.0f);
            RenderMeshIn2D(meshList[GEO_TQUIT], false, 20.f, 5.f, 0.f, -20.0f);
            if (Application::IsKeyPressed(VK_RETURN) && !b_DSENTERState)
            {
                b_DSENTERState = true;
            }
            else if (!Application::IsKeyPressed(VK_RETURN) && b_DSENTERState)
            {
                Application::sceneManager->SetPreviousScene(Application::sceneManager->GetCurrentScene());
                Application::sceneManager->SetCurrentScene(Application::sceneManager->GetLoadingScreen());
                b_DSENTERState = false;
            }
            break;
        }
        case MENU:
        {
            RenderMeshIn2D(meshList[GEO_TBORDER], false, 33.f, 5.f, 0.f, -11.0f);
            RenderMeshIn2D(meshList[GEO_TMENU], false, 30.f, 5.f, 0.f, -10.0f);
            RenderMeshIn2D(meshList[GEO_TRESPAWN], false, 25.f, 5.f, 0.f, 0.0f);
            RenderMeshIn2D(meshList[GEO_TQUIT], false, 20.f, 5.f, 0.f, -20.0f);
            if (Application::IsKeyPressed(VK_RETURN) && !b_DSENTERState)
            {
                b_DSENTERState = true;
            }
            else if (!Application::IsKeyPressed(VK_RETURN) && b_DSENTERState)
            {
                Application::sceneManager->SetPreviousScene(Application::sceneManager->GetCurrentScene());
                Application::sceneManager->SetMenuScene(new SceneMenu());
                Application::sceneManager->GetMenuScreen()->Init();
                Application::sceneManager->SetCurrentScene(Application::sceneManager->GetMenuScreen());
                b_DSENTERState = false;
            }
            break;
        }
        case QUIT:
        {
            RenderMeshIn2D(meshList[GEO_TBORDER], false, 33.f, 5.f, 0.f, -21.0f);
            RenderMeshIn2D(meshList[GEO_TQUIT], false, 25.f, 5.f, 0.f, -20.0f);
            RenderMeshIn2D(meshList[GEO_TMENU], false, 25.f, 5.f, 0.f, -10.0f);
            RenderMeshIn2D(meshList[GEO_TRESPAWN], false, 25.f, 5.f, 0.f, 0.0f);
            if (Application::IsKeyPressed(VK_RETURN) && !b_DSENTERState)
            {
                b_DSENTERState = true;
            }
            else if (!Application::IsKeyPressed(VK_RETURN) && b_DSENTERState)
            {
                SharedData::GetInstance()->SD_QuitGame = true;
                b_DSENTERState = false;
            }
            break;
        }
        }
    }

}

void SceneSP3::RenderHUD()
{
    float barscale = 50.f;

    for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
    {
        GameObject* go = (GameObject*)*it;
        if (go->objectType != GameObject::BOSS)
            continue;
        else
        {
            Boss* bo = (Boss*)*it;
            switch (bo->bossType)
            {
            case Boss::GIANTSQUID:
            {
                if ((bo->pos - playerpos).LengthSquared() < 400 * 400)
                {
                    float percentage = (100.f / 1000.f) * (float)bo->getHealth();
                    float healthscale = (barscale / 100) * percentage;
                    float healthtranslate = (50.f / 100.f) * (100.f - percentage);
                    RenderMeshIn2D(meshList[GEO_HUD_HEALTHBAR], false, 50.0f, 5.0f, 0.f, 50.0f);
                    RenderMeshIn2D(meshList[GEO_HUD_BOSSHEALTH], false, healthscale, 5.0f, 0.f - healthtranslate, 50.0f);
                }
                break;
            }
            }

        }
    }

    RenderDeathScreen();
}

void SceneSP3::RenderMinimap()
{
	const float scale = 0.08f;

	Vector2 mPos;
	mPos.Set(80 - 16 - 4, -60 + 16 + 4);

	float angle = 90 + Math::RadianToDegree(atan2(walkCam.GetDir().z, walkCam.GetDir().x));

	RenderMeshIn2D(meshList[GEO_MINIMAP], false, 20, 20,
		mPos.x, mPos.y, angle);

	for (auto it : m_goList)
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

    RenderMeshIn2D(meshList[GEO_MINIMAP_AVATAR], false, 3.f, 3.f,
		mPos.x, mPos.y);
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
    while (m_goList.size() > 0)
    {
        GameObject *go = m_goList.back();
        delete go;
        m_goList.pop_back();
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
