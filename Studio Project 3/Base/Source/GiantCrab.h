#pragma once
#include "Boss.h"

class GiantCrab : public Boss
{
public:
	GiantCrab();
	~GiantCrab();

	enum CrabState
	{
		IDLE = 0,
		ATTACK,
		GRAB,
		AGGRO,
		VORTEX,
		STRAFE,
		NUM_STATE
	};
	struct Cleg
	{
		float m_Urotate = 0;
		float m_Lrotate = 0;
		bool Ulegmax = 0;
		bool Llegmax = 0;
	};
	struct Carm
	{
		Vector3 m_Upos;
		float y_upper = 0;
		float x_upper = 0;
		float x_lower = 0;
	};




	CrabState GetState() const;
	void SetState(const CrabState &state);
	float GetWalkAnim(const unsigned &id = 0);
	//float speed;
	float m_rotate;
	void Move(const double &dist);
	void UpdateGC(double, std::vector<unsigned char>);
	void AnimateGC(double);
	void UpdateArms(double);
	float m_strafetime;


	float a = 250;
	float b = 200;

	Cleg m_leg[8];

	Carm m_Larm;
	Carm m_Rarm;
	float speed;
	float walkAnim;

	hitbox grabArea;
	hitbox2 m_hitbox;




private:
	CrabState state;
};