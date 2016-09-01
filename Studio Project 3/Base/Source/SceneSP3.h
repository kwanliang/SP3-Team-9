#ifndef SCENE_SP3_H
#define SCENE_SP3_H

#include <vector>
#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "CameraV2.h"
#include "WalkCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Particle.h"
#include "DepthFBO.h"
#include "CollisionManager.h"
#include "Minnow.h"
#include "Pufferfish.h"
#include "Fcrab.h"
#include "Cuttlefish.h"
#include "Chimera.h"
#include "Corals.h"
#include "Projectile.h"
#include "Capture.h"
#include "SharedData.h"
#include "LoadHmap.h"
#include "Skipper.h"
#include "Drone.h"
#include "FontData.h"

#include "GiantSquid.h"
#include "Isopod.h"
#include "GiantCrab.h"

static int g_countCapturedMinnow = 0;
static int g_countCapturedPuffer = 0;
static int g_countCapturedCrab = 0;
static int g_countCapturedCuttle = 0;
static int g_countCapturedChimera = 0;

class SceneSP3 : public Scene
{
protected:
	enum KeyStatuses
	{
		KEY_STATUS_CUR = 0,
		KEY_STATUS_DOWN,
		KEY_STATUS_UP,
		KEY_STATUS_TEMP,
	};

public:
    SceneSP3();
    virtual ~SceneSP3();

    virtual void Init();
    virtual void Update(double dt);
	void Render();
    virtual void Exit();

    virtual void RenderPassGPass() {};
    virtual void RenderPassMain() {};
    virtual void RenderWorld() {};
	virtual void RenderMinimap();
    virtual void RenderHUD();
    void RenderDeathScreen();
	void RenderSquadHUD();
    void RenderObjectiveHUD();
    void RenderCoralHUD(); 

	void UpdateTravel();
    void UpdateSeaCreatures(double dt);
    void UpdateProjectile(double dt);
	void UpdatePuffer(double dt);
	void UpdateCaptured(double dt);
	void UpdateCapturedPuff(int damage);
	void UpdateSquadFire(double dt);
    void UpdateSpawner(double dt);
	void UpdateStunned(double dt);
    void UpdateDeathScreen(double dt);

    void RenderText(Mesh* mesh, std::string text, Color color);
    void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, const FontData &fd);
    void RenderMeshIn2D(Mesh *mesh, bool enableLight, float sizex = 5.0f, float sizey = 5.0f, float x = 0.0f, float y = 0.0f, float rot = 0.f);
    void RenderMesh(Mesh *mesh, bool enableLight);

    void RenderParticles();
    void UpdateParticles(double dt);
	void UpdateSP(ParticleObject*, double);
    ParticleObject* GetParticle();

    Minnow* FetchMinnow();
    Pufferfish* FetchPuffer();
    Fcrab* FetchFCrab();
    Chimera* FetchChimera();
    Cuttlefish* FetchCuttle();
	Drone* FetchDrone();
    Coral* FetchCoral();
    Projectile* FetchPO();
    DamageText* FetchTO();
    void RenderFO(SeaCreature *fo);
    void RenderPO(Projectile *po);
    void RenderTO(DamageText *to);
    void RenderCoral(Coral *co);
    void RenderSquad();

    void RenderLoop();

    float LookAtPlayer(Vector3 playerpos, Vector3 otherpos);

    float sliderTranslate;
    double t_stuckPrevention;

    float fogThickness;
    float blendFactor;

    GiantSquid* giantSquid;
    GiantCrab* giantCrab;

    Isopod* isopod;

    enum DEATHSELECT
    {
        RESPAWN,
        MENU,
        QUIT,
        NUM_DEATHSELECT
    }DeathSelect;

	bool GetKeyState(const unsigned char &key, const KeyStatuses &status = KEY_STATUS_CUR);

private:

	//Information for storing keystates. (State; Pressed; Released; Temp) 
	bool keyStates[4][255];

	void UpdateKeys();
	void UpdatePauseFunction();
	void UpdatePauseScreen(double dt);

	void RenderPauseScreen();

    // Shadow
    //DepthFBO m_lightDepthFBO;
    //Mtx44 m_lightDepthProj;
    //Mtx44 m_lightDepthView;
 
    bool bLightEnabled;
	float m_fireRate;
    //Terrain
 
	//Fish test
	Capture fishy;

protected:
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,

		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,

		U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,

		U_LIGHT2_TYPE,
		U_LIGHT2_POSITION,
		U_LIGHT2_COLOR,
		U_LIGHT2_POWER,
		U_LIGHT2_KC,
		U_LIGHT2_KL,
		U_LIGHT2_KQ,
		U_LIGHT2_SPOTDIRECTION,
		U_LIGHT2_COSCUTOFF,
		U_LIGHT2_COSINNER,
		U_LIGHT2_EXPONENT,

		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE_ENABLED1,
		U_COLOR_TEXTURE,
		U_COLOR_TEXTURE1,
		U_IS_GUI,
		U_TEXT_COLOR,
		U_FOG_COLOR,
		U_FOG_START,
		U_FOG_END,
		U_FOG_THICKNESS,
		U_FOG_DENSITY,
		U_FOG_TYPE,
		U_FOG_ENABLE,

		U_LIGHT_DEPTH_MVP_GPASS,
		U_LIGHT_DEPTH_MVP,
		U_SHADOW_MAP,

		U_SHADOW_COLOR_TEXTURE_ENABLED,
		U_SHADOW_COLOR_TEXTURE,
		U_SHADOW_COLOR_TEXTURE_ENABLED1,
		U_SHADOW_COLOR_TEXTURE1,
		U_SHADOW_COLOR_TEXTURE_ENABLED2,
		U_SHADOW_COLOR_TEXTURE2,

		U_TEXTURE_BLEND_ENABLE,
		U_TEXTURE_BLEND_FACTOR,

		U_TOTAL,
	};

	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_CUBE,
		GEO_CROSSHAIR,
		GEO_TEXT,
		GEO_BALL,
		GEO_BALL2,

        // TERRAIN
		GEO_TERRAIN0,
		GEO_TERRAIN1,
		GEO_TERRAIN2,
		GEO_TERRAIN3,
		GEO_TERRAIN4,

        // OBJECTS
        GEO_FISHMODEL,
        GEO_FISHTAIL,
        GEO_LASER,

        GEO_SQUIDBODY,
        GEO_SQUIDTENTACLENODE,
        GEO_SQUIDTENTACLEEND,

        GEO_FSHARK_LJAW,
        GEO_FSHARK_UJAW,
        GEO_FSHARK_NODE,
        GEO_FSHARK_TAIL,

        GEO_ISOPOD_BODY,
        GEO_ISOPOD_LEG,
        GEO_ISOPOD_CLAW,

        GEO_ISOPOD_DRONE,

        GEO_CHIMERA_BODY,
        GEO_CHIMERA_FFLIP,
        GEO_CHIMERA_BFLIP,

        GEO_MINNOW,
        GEO_PUFFER,
        GEO_CUTTLE,

        GEO_FCRABBODY,
        GEO_FCRABLEG,
        GEO_FCRABCLAW,

        GEO_CAP_CHIMERA_BODY,
        GEO_CAP_CHIMERA_FFLIP,
        GEO_CAP_CHIMERA_BFLIP,

        GEO_CAP_MINNOW,
        GEO_CAP_PUFFER,
        GEO_CAP_CUTTLE,

        GEO_CAP_FCRABBODY,
        GEO_CAP_FCRABLEG,
        GEO_CAP_FCRABCLAW,

        GEO_CRAB_BODY,
        GEO_CRAB_ARM_UPPER,
        GEO_CRAB_ARM_MID,
        GEO_CRAB_ARM_LOWER,
        GEO_CRAB_LEG_UPPER,
        GEO_CRAB_LEG_LOWER,

        GEO_CORAL,
        GEO_CORAL2,

        // HUD
        GEO_PLAYERHEALTH,
        GEO_PLAYERSTAMINA,

        GEO_MINIMAP,
        GEO_MINIMAP_AVATAR,
        GEO_MINIMAP_MINNOW,
        GEO_MINIMAP_CUTTLE,
        GEO_MINIMAP_CRAB,
        GEO_MINIMAP_GHOSTSHARK,
        GEO_MINIMAP_PUFFER,
        GEO_MINIMAP_BOSS,

        GEO_PAUSEMENU,

        GEO_STATIC,

        GEO_BUBBLE,
        GEO_VACUUM,

        GEO_HUD_HEALTHBAR,
        GEO_HUD_BOSSHEALTH,

        GEO_TTITLE,
        GEO_TMENU,
        GEO_TSTART,
        GEO_TQUIT,
        GEO_TLOADING,
        GEO_MFISH,

        GEO_TBORDER,
        GEO_TLAYER,
        GEO_TDIED,
        GEO_TRESPAWN,
        GEO_TTOMENU,

        GEO_TPAUSE,
        GEO_TRESUME,

        GEO_INSTRUCTION,
        GEO_INSTRUCTION2,

        GEO_MINNOW_HUD,
        GEO_CUTTLE_HUD,
        GEO_CRAB_HUD,
        GEO_CHIMERA_HUD,
        GEO_PUFFER_HUD,

		NUM_GEOMETRY,
	};
	enum RENDER_PASS
	{
		RENDER_PASS_PRE,
		RENDER_PASS_MAIN,
	};

	// Game Object
	std::vector<GameObject*> seaList;
    std::vector<GameObject*> coralList;
    std::vector<GameObject*> projectileList;
    std::vector<GameObject*> captureList;

	CameraV2 *currentCam;
	WalkCamera walkCam;

    // Particles 
    std::vector<ParticleObject*> particleList; // Used to store
    Vector3 m_gravity;      // Gravity affecting the particles
    int m_particleCount;    // Number of particles
	int m_spCount;//suspended particle count

	//ligts and rendering
	MS modelStack;
	MS viewStack;
	MS projectionStack;
	DepthFBO m_lightDepthFBO;
	Mtx44 m_lightDepthProj;
	Mtx44 m_lightDepthView;
	RENDER_PASS m_renderPass;
	Light lights[1];
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_gPassShaderID;
	unsigned m_parameters[U_TOTAL];

	//level stuff
	hitbox m_travelzonedown;
	hitbox m_travelzoneup;

	std::vector<unsigned char> m_heightMap[5];
	float fps;
	//player test stuff
	Vector3 playerpos;
	hitbox2 player_box;
	float val = 0;
	Vector3 fishRot;
	float	fish_tailrot;
	bool	fish_tailmax;
	Vector3 fishVel;

    Skipper* skipper;
    Minnow* minnowLeader;

	bool isGamePaused;
	int pauseChoice;

	FontData fontData;

    // Spawner
    Spawner MinnowLeaderSpawner;
    Spawner MinnowSpawner;
    Spawner PufferfishSpawner;
    Spawner FCrabSpawner;
    Spawner ChimeraSpawner;
    Spawner CuttlefishSpawner;
    Spawner IsopodDroneSpawner;
    Spawner CoralSpawner;

    //DamageText text;
    std::vector<DamageText*> m_textList;
};

#endif