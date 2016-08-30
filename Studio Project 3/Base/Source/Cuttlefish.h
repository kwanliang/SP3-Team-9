#ifndef CUTTLEFISH_H
#define CUTTLEFISH_H

#include "SeaCreature.h"
#include "SharedData.h"

class Cuttlefish : public SeaCreature
{
private:
    bool m_IsShoot;
    double m_ShootBufferTime;

public:
	enum CUTTLE_STATE
	{
		IDLE,
		ATTACKING,
		SINK,
		STATE_TOTAL
	};

	Cuttlefish();
	~Cuttlefish();

	void UpdateCuttle(double dt);
	CUTTLE_STATE ctstate;
	float rotate;
	bool jet;

    bool getIsShoot();
    void setIsShoot(bool m_IsShoot);

    bool getShootBufferTime();
    void setShootBufferTime(bool m_ShootBufferTime);
};

#endif