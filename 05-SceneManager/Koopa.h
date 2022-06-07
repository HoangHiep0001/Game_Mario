#pragma once
#include "GameObject.h"
#include "Timer.h"
#include "Mario.h"

#define KOOPA_GRAVITY 0.0006f
#define KOOPA_WALKING_SPEED 0.035f
#define KOOPA_SHELL_MOVING_SPEED 0.16f
#define PARAKOOPA_DEFLECT_SPEED_Y 0.22f
#define KOOPA_SHELL_DEFLECT_X 0.035f
#define KOOPA_SHELL_DEFLECT_Y 0.28f

#define KOOPA_DIE_TIMEOUT 300

#define KOOPA_SHELL_TIME 8000
#define KOOPA_VIBRATION_TIME 3000

#define KOOPA_STATE_DIE 199


#pragma region KOOPA_BBOX_SIZE

#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 26
#define KOOPA_SHELL_BBOX_HEIGHT 15

#define KOOPA_SHELL_OFFSET_TOP 2
#define KOOPA_HELD_SHELL_POS_X_RIGHT 17
#define KOOPA_HELD_SHELL_POS_X_LEFT 5
#define KOOPA_HELD_SHELL_POS_Y 3
#define KOOPA_HELD_SHELL_POS_Y_SMALL 1

#define	KOOPA_STATE_SHELL_BY_ATTACK	201
#define	KOOPA_STATE_WALKING			200
#define	KOOPA_STATE_SHELL			202
#define	KOOPA_STATE_SHELL_MOVING	203
#define KOOPA_STATE_BEING_HELD		204
#define KOOPA_STATE_VIBRATE			205
#define PARAKOOPA_STATE_NORMAL			206

#pragma endregion

#pragma region KOOPA_ANIMATION_ID

#define ID_ANI_RED_KOOPA_WALKING_RIGHT			710
#define ID_ANI_RED_KOOPA_WALKING_LEFT			711
#define ID_ANI_RED_KOOPA_SHELL_PRONE			712
#define ID_ANI_RED_KOOPA_SHELL_SUPINE			713 // or DIE
#define ID_ANI_RED_KOOPA_VIBRATE_PRONE			714
#define ID_ANI_RED_KOOPA_VIBRATE_SUPINE			715
#define ID_ANI_RED_KOOPA_SHELL_MOVING_PRONE		716
#define ID_ANI_RED_KOOPA_SHELL_MOVING_SUPINE	717

#define ID_ANI_GREEN_KOOPA_WALKING_RIGHT		731
#define ID_ANI_GREEN_KOOPA_WALKING_LEFT			732
#define ID_ANI_GREEN_KOOPA_SHELL_PRONE			733
#define ID_ANI_GREEN_KOOPA_SHELL_SUPINE			734 // or DIE
#define ID_ANI_GREEN_KOOPA_VIBRATE_PRONE		735
#define ID_ANI_GREEN_KOOPA_VIBRATE_SUPINE		736
#define ID_ANI_GREEN_KOOPA_SHELL_MOVING_PRONE	737
#define ID_ANI_GREEN_KOOPA_SHELL_MOVING_SUPINE	738

#define ID_ANI_GREEN_PARAKOOPA_WALKING_RIGHT	739
#define ID_ANI_GREEN_PARAKOOPA_WALKING_LEFT		740

#pragma endregion

class CKoopa : public CGameObject
{
protected:
	float ax;
	float ay;

	bool isBeingHeld;
	bool isSupine;

	CMario* player;

	CTimer* shellTime = new CTimer(KOOPA_SHELL_TIME);
	CTimer* vibrationTime = new CTimer(KOOPA_VIBRATION_TIME);

	vector<LPGAMEOBJECT> subItems;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithPandoraBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithMagicCoinBrick(LPCOLLISIONEVENT e);

public:
	CKoopa(float x, float y, Type type, CMario* player);
	virtual void SetState(int state);

	virtual void ChangeDirection();
	void SetPositionFollowPlayer();

	int GetAniRed();
	int GetAniGreen();
};

