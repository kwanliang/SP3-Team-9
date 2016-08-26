#pragma once
#include "Boss.h"

class GiantCrab : public Boss
{
public:
	GiantCrab();
	~GiantCrab();

	enum CrabState
	{
		STATE_IDLE = 0,
		STATE_MOVE,
		STATE_ATTACK,
		STATE_GRAB,
		STATE_AGGRESSIVE,
	};

	CrabState GetState() const;
	void SetState(const CrabState &state);
	float GetWalkAnim(const unsigned &id = 0);
	void Move(const double &dist);

	float walkAnim;

private:
	CrabState state;
};