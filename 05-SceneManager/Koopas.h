#pragma once
#include "GameObject.h"


#define KOOPAS_WALKING_SPEED 0.03f;
#define KOOPAS_JUMP_SPEED_Y 0.25f
#define KOOPAS_GRAVITY 0.002f
#define KOOPAS_TORTOISESHELL 0.15f
#define KOOPAS_VY 0.001f
#define KOOPAS_DIE_VY 0.35f
//BBOX
#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 27
#define KOOPAS_BBOX_LIVING 15.7
//STATE
#define KOOPAS_STATE_WALKING 0
#define KOOPAS_STATE_FLYLING 1
#define KOOPAS_STATE_TORTOISESHELL_UP 2
#define KOOPAS_STATE_TORTOISESHELL_DOWN 3
#define KOOPAS_STATE_DIE_UP 4
#define KOOPAS_STATE_DIE_DOWN 5
#define KOOPAS_STATE_LIVING_UP 6
#define KOOPAS_STATE_LIVING_DOWN 7
#define KOOPAS_STATE_LIVE_FOOT_UP 8
#define KOOPAS_STATE_LIVE_FOOT_DOWN 9



//ANI
#define KOOPAS_ANI_BULE_WALKING_RIGHT 5500
#define KOOPAS_ANI_BULE_WALKING_LEFT 5501
#define KOOPAS_ANI_BULE_FLYLING_RIGHT 5502
#define KOOPAS_ANI_BULE_FLYLING_LEFT 5503
#define KOOPAS_ANI_BULE_TORTOISESHELL_UP 5504
#define KOOPAS_ANI_BULE_TORTOISESHELL_DOWN 5505
#define KOOPAS_ANI_BULE_DIE_UP 5506
#define KOOPAS_ANI_BULE_DIE_DOWN 5507
#define KOOPAS_ANI_BULE_LIVE_FOOT_UP 5508
#define KOOPAS_ANI_BULE_LIVE_FOOT_DOWN 5509

#define KOOPAS_ANI_RED_WALKING_RIGHT 5550 
#define KOOPAS_ANI_RED_WALKING_LEFT 5551
#define KOOPAS_ANI_RED_FLYLING_RIGHT 5552
#define KOOPAS_ANI_RED_FLYLING_LEFT 5553
#define KOOPAS_ANI_RED_TORTOISESHELL_UP 5554
#define KOOPAS_ANI_RED_TORTOISESHELL_DOWN 5555
#define KOOPAS_ANI_RED_DIE_UP 5556
#define KOOPAS_ANI_RED_DIE_DOWN 5557
#define KOOPAS_ANI_RED_LIVE_FOOT_UP 5558
#define KOOPAS_ANI_RED_LIVE_FOOT_DOWN 5559

#define KOOPAS_RED 0
#define KOOPAS_BULE 1

#define KOOPAS_JUMP_Y 35
#define KOOPAS_BIG_HOLD 5
#define KOOPAS_SMALL_HOLD 3

#define KOOPAS_TIME_DIE 1000
class CKoopas : public CGameObject
{
protected:
	float ax;
	float ay;
	int apperance;
	ULONGLONG die_start;
	DWORD time = 0;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CKoopas(float x, float y, int app);
	virtual void SetState(int state);
	int GetApperance() { return apperance; }
};

