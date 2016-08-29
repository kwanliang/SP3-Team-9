#ifndef ISOPOD_H
#define	ISOPOD_H


#include "Boss.h"


class Isopod :public Boss
{
public:
	struct Nest
	{
		bool active = true;
		Vector3 pos;
		float resetTime;
	};
	struct Leg
	{
		float yaw = 0;
		float roll = 0;

	};
	enum Istate
	{
		IDLE,
		AGGRO,
		CHARGE,
		COLLIDING,
		STATE_END

	};

	Isopod();
	Isopod(int m_health, BOSS_TYPE bossType,
		Vector3 pos, Vector3 vel, Vector3 scale, bool active);
	~Isopod();

	void UpdateIsopod(double, std::vector<unsigned char>);
	void AnimateIsopod(double);


	Istate m_state;
	Leg m_Rleg[6];
	Leg m_Lleg[6];
	Nest m_nest_A;
	Nest m_nest_B;

	Nest* m_targetnest;

private:










};













#endif;