#pragma once
#include "GameObject.h"
#include "Timer.h"

#define TAIL_BBOX_WIDTH 10
#define TAIL_BBOX_HEIGHT 6

#define TAIL_OFFSET_Y 5
#define TAIL_OFFSET_X_LEFT	9
#define TAIL_OFFSET_X_RIGHT	11

#define TAIL_SETNX 1

class CTail : public CGameObject
{
public:
	BOOLEAN attackIsOn;
	CTimer* attackTime;

	vector<LPGAMEOBJECT> effects;

	CTail(CTimer* attackTime)
	{
		attackIsOn = false;
		this->attackTime = attackTime;
	}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void Effect();
	void Attack();
	void OnCollisionWithGoomba(LPGAMEOBJECT e);
	void OnCollisionWithKoopa(LPGAMEOBJECT e);
	void OnCollisionWithPandoraBrick(LPGAMEOBJECT e);
	void OnCollisionWithMagicCoinBrick(LPGAMEOBJECT e);
};
