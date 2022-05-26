#pragma once
#include "GameObject.h"

#define MUSHROOM_GRAVITY 0.001f
#define MUSHROOM_MOVING_SPEED 0.05f
#define MUSHROOM_BOUNCE_SPEED 0.03f

#define MUSHROOM_STATE_MOVING 100

#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16

#define ID_ANI_SUPER_MUSHROOM 888
#define ID_ANI_1_UP_MUSHROOM 889

class CMushroom : public CGameObject
{
protected:
	float ay;

	float highestPos;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CMushroom(float x, float y, Type type) : CGameObject(x, y, type) {
		ay = 0;
		vy = -MUSHROOM_BOUNCE_SPEED;
		highestPos = y - MUSHROOM_BBOX_HEIGHT;
	}
};

