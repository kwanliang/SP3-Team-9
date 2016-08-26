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
		AGGRESSIVE,
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
		float y_upper = 0;
		float x_upper = 0;
		float x_lower = 0;
	};




	CrabState GetState() const;
	void SetState(const CrabState &state);
	float GetWalkAnim(const unsigned &id = 0);
	float speed;
	void Move(const double &dist);
	void updateGC(double);
	void AnimateGC(double);
	Cleg m_leg[8];

	Carm m_Larm;

	float walkAnim;

private:
	CrabState state;
};