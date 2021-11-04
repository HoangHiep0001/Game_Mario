#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.2f

#define MARIO_ACCEL_WALK_X	0.0005f
#define MARIO_ACCEL_RUN_X	0.0007f

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_RUN_SPEED_Y	0.6f

#define MARIO_GRAVITY			0.002f

#define MARIO_JUMP_DEFLECT_SPEED  0.4f

//state
#pragma region State
#define MARIO_STATE_DIE				0
#define MARIO_STATE_IDLE			100
#define MARIO_STATE_WALKING_RIGHT	200
#define MARIO_STATE_WALKING_LEFT	300
#define MARIO_STATE_JUMP			400
#define MARIO_STATE_JUMP_MAX 500
#define MARIO_STATE_WALKING_RIGHT_FAST 600
#define MARIO_STATE_WALKING_LEFT_FAST 700
#define MARIO_STATE_SIT	800
#define MARIO_STATE_STOP	900
#define MARIO_STATE_FLY	1000
#define MARIO_STATE_FLYLING	1100
#define MARIO_STATE_LANDING	1200
#define MARIO_STATE_HOLD 1300
#define MARIO_STATE_STONE_KOOPAS 1400
#define MARIO_STATE_FALL_DOWN	1500

#pragma endregion


#pragma region ANIMATION_ID

#define ID_ANI_MARIO_DIE 399

// SMALL MARIO
#pragma region Mario_small
#define MARIO_ANI_SMALL_IDLE_RIGHT		501
#define MARIO_ANI_SMALL_IDLE_LEFT			502
#define MARIO_ANI_SMALL_WALKING_RIGHT		503
#define MARIO_ANI_SMALL_WALKING_LEFT		504
#define MARIO_ANI_SMALL_WALKING_FAST_RIGHT 509
#define MARIO_ANI_SMALL_WALKING_FAST_LEFT 510
#define MARIO_ANI_SMALL_JUMP_IDLE_RIGHT 507
#define MARIO_ANI_SMALL_JUMP_IDLE_LEFT 508
#define MARIO_ANI_SMALL_FALL_DOWN_RIGHT 507
#define MARIO_ANI_SMALL_FALL_DOWN_LEFT 508
#define MARIO_ANI_SMALL_STOP_IDLE_RIGHT 505
#define MARIO_ANI_SMALL_STOP_IDLE_LEFT 506
#define MARIO_ANI_SMALL_FLY_IDLE_RIGHT 511
#define MARIO_ANI_SMALL_FLY_IDLE_LEFT 512
#define MARIO_ANI_SMALL_HOLD_RIGHT 513
#define MARIO_ANI_SMALL_HOLD_LEFT 514
#define MARIO_ANI_SMALL_STONE_KOOPAS_RIGHT 515
#define MARIO_ANI_SMALL_STONE_KOOPAS_LEFT 516

#pragma endregion

#pragma region Mario Big
#define MARIO_ANI_BIG_IDLE_RIGHT		400
#define MARIO_ANI_BIG_IDLE_LEFT			401
#define MARIO_ANI_BIG_WALKING_RIGHT	404
#define MARIO_ANI_BIG_WALKING_LEFT	405		
#define MARIO_ANI_BIG_WALKING_FAST_RIGHT 416
#define MARIO_ANI_BIG_WALKING_FAST_LEFT 417
#define MARIO_ANI_BIG_JUMP_IDLE_RIGHT 410
#define MARIO_ANI_BIG_JUMP_IDLE_LEFT 411
#define MARIO_ANI_BIG_FALL_DOWN_RIGHT 410
#define MARIO_ANI_BIG_FALL_DOWN_LEFT 411
#define MARIO_ANI_BIG_SIT_RIGHT 409
#define MARIO_ANI_BIG_SIT_LEFT 408
#define MARIO_ANI_BIG_STOP_IDLE_RIGHT 414
#define MARIO_ANI_BIG_STOP_IDLE_LEFT 415
#define MARIO_ANI_BIG_FLY_IDLE_RIGHT 418
#define MARIO_ANI_BIG_FLY_IDLE_LEFT 419
#define MARIO_ANI_BIG_HOLD_RIGHT 420
#define MARIO_ANI_BIG_HOLD_LEFT 421
#define MARIO_ANI_BIG_STONE_KOOPAS_RIGHT 422
#define MARIO_ANI_BIG_STONE_KOOPAS_LEFT 423

#pragma endregion

#pragma endregion

#define GROUND_Y 160.0f




#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 25
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 13


#define MARIO_UNTOUCHABLE_TIME 2500

class CMario : public CGameObject
{
	BOOLEAN isSitting;
	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 

	int level; 
	int untouchable; 
	ULONGLONG untouchable_start;
	BOOLEAN isOnPlatform;
	int coin; 
	//xét va cham
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithGround(LPCOLLISIONEVENT e);

	int GetAniIdBig();
	int GetAniIdSmall();

public:
	CMario(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY; 

		level = MARIO_LEVEL_BIG;
		untouchable = 0;
		untouchable_start = -1;
		isOnPlatform = false;
		coin = 0;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	int IsCollidable()
	{ 
		return (state != MARIO_STATE_DIE); 
	}

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable==0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetLevel(int l);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};