#pragma once
#include "GameObject.h"
#include "Timer.h"

class CTail : public CGameObject
{
public:
	BOOLEAN attackIsOn;
	CTimer* attackTime;

	CTail(CTimer* attackTime)
	{
		attackIsOn = false;
		this->attackTime = attackTime;
	}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void Attack();
	void OnCollisionWithGoomba(LPGAMEOBJECT e);
	void OnCollisionWithKoopa(LPGAMEOBJECT e);
	void OnCollisionWithPandoraBrick(LPGAMEOBJECT e);
	void OnCollisionWithMagicCoinBrick(LPGAMEOBJECT e);
};
