#ifndef FRILLEDSHARK_H
#define	FRILLEDSHARK_H


#include "Boss.h"


class FrilledShark :public Boss
{
public:
	struct FSnode
	{
		Vector3 pos;
		float yaw = 0;
		float pitch = 0;

	};
	enum FSstate
	{
		IDLE,
		AGGRO,
		CHARGE,
		STRAFE,
		GRAPPLE,
		COLLIDING,
		STATE_END

	};

	FrilledShark();
	FrilledShark(int m_health ,  BOSS_TYPE bossType, 
					Vector3 pos, Vector3 vel, Vector3 scale, bool active);
	~FrilledShark();
	
	void UpdateFrilledShark(double,std::vector<unsigned char>);
	void AnimateFrilledShark(double); 
	void UpdateWhiskers();
	void HandleCollision();
	float Ljaw_rotate;
	double m_strafeTime;
	FSstate m_state;
	FSnode m_node[5];


	hitbox2 m_Rwhisker;
	hitbox2 m_Lwhisker;
	

private:










};













#endif;