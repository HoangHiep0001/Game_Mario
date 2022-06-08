#pragma once
#include "GameObject.h"
#include "Timer.h"

#define TAIL_HIT_EFFECT_TIME 200

#define ID_ANI_TAIL_HIT_EFFECT 902

class CEffectTail : public CGameObject
{
	CTimer* effectTime = new CTimer(TAIL_HIT_EFFECT_TIME);

public:
	CEffectTail(float x, float y)
	{
		this->x = x;
		this->y = y;
		effectTime->Start();
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) { l = t = r = b = 0; }
};

