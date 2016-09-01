
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include "SceneManager.h"
#include "irrKlang.h"

using namespace irrklang;

class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);
    static bool IsMousePressed(unsigned short key);
	static bool GetMouseUpdate();
	//Declare variables to store the last and current mouse position
	static double mouse_last_x, mouse_last_y, mouse_current_x, mouse_current_y, mouse_diff_x, mouse_diff_y;
	static double camera_yaw, camera_pitch;

    static int GetWindowWidth();
    static int GetWindowHeight();
    static int m_window_width;
    static int m_window_height;
	static double GetAspectRatio();

	static SceneManager *sceneManager;

    static ISoundEngine *SoundEngine;
    //static ISound *BGM1;
    //static ISound *BGM2;
    //static ISound *BGM3;
    //static ISound *BGM4;
    //static ISound *MenuSound;
    //static ISound *BoostSound;
    //static ISound *FCrabSound;
    //static ISound *DroneBiteSound;
    //static ISound *FrilledSharkSound1;
    //static ISound *FrilledSharkSound2;
    //static ISound *ChimeraSound;
    //static ISound *GiantCrabSound;
    //static ISound *GiantCrabSoundAggro;
    //static ISound *GiantCrabSoundVortex;
    //static ISound *GiantSquid;
    //static ISound *HitSound;
    //static ISound *InkSound;
    //static ISound *IsopodSound;
    //static ISound *MoveSound;
    static ISound *ShootSound;
    //static ISound *VacuumSound;
    //static ISound *PopSound;

private:
	Application();
	~Application();

	// Declare the window width and height as constant integer
	const static int m_window_deadzone = 100;
	//Declare a window object
	StopWatch m_timer;
};

#endif