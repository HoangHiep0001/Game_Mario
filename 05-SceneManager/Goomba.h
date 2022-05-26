#pragma once
#include "GameObject.h"
#include "Timer.h"

#define GOOMBA_WALKING_SPEED 0.035f
#define GOOMBA_GRAVITY 0.0006f
#define GOOMBA_DIE_DEFLECT_SPEED_Y 0.22f
#define PARAGOOMBA_HIGH_FLYING_GRAVITY 0.0004f

#define PARAGOOMBA_LOW_FLYING_SPEED_Y 0.07f
#define PARAGOOMBA_HIGH_FLYING_SPEED_Y 0.18f

#define GOOMBA_DIE_TIMEOUT 250
#define PARAGOOMBA_WALK_TIME 800
#define PARAGOOMBA_REDIRECTION_DELAY 300
#define PARAGOOMBA_CHASING_TIME 15000


#pragma region GOOMBA_STATE

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE_BY_CRUSH 200
#define GOOMBA_STATE_DIE_BY_ATTACK 201

#define PARAGOOMBA_STATE_FLY_LOW 298
#define PARAGOOMBA_STATE_FLY_HIGH 297
#define PARAGOOMBA_STATE_NORMAL 296

#pragma endregion

#pragma region GOOMBA_BBOX_SIZE

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 15
#define GOOMBA_DIE_BBOX_HEIGHT 8

#define PARAGOOMBA_BBOX_WIDTH 20
#define PARAGOOMBA_BBOX_HEIGHT 15

#define GOOMBA_DIE_OFFSET_LEFT 7
#define GOOMBA_DIE_OFFSET_TOP 1
#define GOOMBA_WALKING_OFFSET_TOP 8
#define PARAGOOMBA_DIE_OFFSET_TOP 5

#pragma endregion

#pragma region GOOMBA_ANIMATION_ID

#define ID_ANI_GOOMBA_WALKING		702
#define ID_ANI_GOOMBA_DIE_BY_CRUSH	703
#define ID_ANI_GOOMBA_DIE_BY_ATTACK	704

#define ID_ANI_PARAGOOMBA_WINGS_WALKING 705
#define ID_ANI_PARAGOOMBA_NORMAL_WALKING 708
#define ID_ANI_PARAGOOMBA_FLAP_WINGS_QUICKLY 706
#define ID_ANI_PARAGOOMBA_FLAP_WINGS_SLOWLY	707
#define ID_ANI_PARAGOOMBA_DIE_BY_CRUSH 709
#define ID_ANI_PARAGOOMBA_DIE_BY_ATTACK 750

#pragma endregion

class CGoomba : public CGameObject
{
protected:
	float ax;				
	float ay; 

	int lowFlyingCounter;
	bool lostWings;

	CTimer* dieTime = new CTimer(GOOMBA_DIE_TIMEOUT);
	CTimer* walkTime = new CTimer(PARAGOOMBA_WALK_TIME);
	CTimer* redirectionDelay = new CTimer(PARAGOOMBA_REDIRECTION_DELAY);
	CTimer* chasingTime = new CTimer(PARAGOOMBA_CHASING_TIME);

	float GetSpeedX();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public: 	
	CGoomba(float x, float y, Type type);
	virtual void SetState(int state);

	bool HasWings() { return !lostWings; }
};