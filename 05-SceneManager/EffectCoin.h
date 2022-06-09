#pragma once
#include "GameObject.h"

#define EFFECT_COIN_SPEED_Y 0.14f

#define EFFECT_COIN_HIGHEST_OFFSET 50
#define EFFECT_COIN_LOWEST_OFFSET 10

#define ID_ANI_EFFECT_COIN 903

class CEffectCoin : public CGameObject
{
	float highest;
	float lowest;
public:
	CEffectCoin (float x, float y, Type type) : CGameObject(x, y, type)
	{
		highest = y - EFFECT_COIN_HIGHEST_OFFSET;
		lowest = y - EFFECT_COIN_LOWEST_OFFSET;
		vy = -EFFECT_COIN_SPEED_Y;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) { l = t = r = b = 0; }
};

