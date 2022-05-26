#pragma once
#include "GameObject.h"

#define KOOPA_GRAVITY 0.0006f
#define KOOPA_WALKING_SPEED 0.035f
#define KOOPA_SHELL_MOVING_SPEED 0.16f

#define KOOPA_DIE_TIMEOUT 300

#define KOOPA_STATE_WALKING 100
#define KOOPA_STATE_DIE 200


#pragma region KOOPA_BBOX_SIZE

#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 26
#define KOOPA_SHELL_BBOX_HEIGHT 15

#define KOOPA_SHELL_OFFSET_TOP 3

#define	KOOPA_STATE_DIE_BY_ATTACK	201
#define	KOOPA_STATE_WALKING			200
#define	KOOPA_STATE_SHELL			202
#define	KOOPA_STATE_SHELL_MOVING	203

#pragma endregion

#pragma region KOOPA_ANIMATION_ID

#define ID_ANI_KOOPA_WALKING_RIGHT			710
#define ID_ANI_KOOPA_WALKING_LEFT			711
#define ID_ANI_KOOPA_SHELL_PRONE			712
#define ID_ANI_KOOPA_SHELL_SUPINE			713 // or DIE
#define ID_ANI_KOOPA_VIBRATE_PRONE			714
#define ID_ANI_KOOPA_VIBRATE_SUPINE			715
#define ID_ANI_KOOPA_SHELL_MOVING_PRONE		716
#define ID_ANI_KOOPA_SHELL_MOVING_SUPINE	717

#pragma endregion

class CKoopa : public CGameObject
{
protected:
	float ax;
	float ay;

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

public:
	CKoopa(float x, float y, Type type);
	virtual void SetState(int state);

	virtual void ChangeDirection();
};

