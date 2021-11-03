#pragma once
#include "GameObject.h"

#define GOOMBA_GRAVITY 0.002f
#define GOOMBA_WALKING_SPEED 0.05f
#define GOOMBA_JUMP_FLY_SPEED_Y	0.3f
#define GOOMBA_JUMP_SPEED_Y	0.25f

#define GOOMBA_BBOX_WING 20
#define GOOMBA_BBOX_FLYING_Y 24
#define GOOMBA_BBOX_X_Y 16
#define GOOMBA_BBOX_Y 7

// state
#define GOOMBA_STATE_WALKING 0
#define GOOMBA_STATE_FLYLING 1
#define GOOMBA_STATE_WALKING_WING 2
#define GOOMBA_STATE_DIE 3
#define GOOMBA_STATE_WALKING_DOWN 4
#define GOOMBA_STATE_FLYLING_DOWN 5
#define GOOMBA_STATE_WALKING_WING_DOWN 6
// ani
#define GOOMBA_ANI_RED_WALKING 5050
#define GOOMBA_ANI_RED_FLYLING 5051
#define GOOMBA_ANI_RED_WALKING_WING 5052
#define GOOMBA_ANI_RED_DIE 5053
#define GOOMBA_ANI_RED_WALKING_DOWN 5054
#define GOOMBA_ANI_RED_FLYLING_DOWN 5055
#define GOOMBA_ANI_RED_WALKING_WING_DOWN 5056

#define GOOMBA_ANI_THERE_WALKING 5000
#define GOOMBA_ANI_THERE_FLYLING 5001
#define GOOMBA_ANI_THERE_WALKING_WING 5002
#define GOOMBA_ANI_THERE_DIE 5003
#define GOOMBA_ANI_THERE_WALKING_DOWN 5004
#define GOOMBA_ANI_THERE_FLYLING_DOWN 5005
#define GOOMBA_ANI_THERE_WALKING_WING_DOWN 5006


// APP
#define GOOMBA_RED 0
#define GOOMBA_THERE 1

#define GOOMBA_DIE_TIMEOUT 500
class CGoomba : public CGameObject
{
protected:
	float ax;				
	float ay; 
	int apperance;
	ULONGLONG die_start;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public: 	
	CGoomba(float x, float y,int app);
	virtual void SetState(int state);
	int GetApperance() { return apperance; }
};