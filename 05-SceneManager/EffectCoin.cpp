#include "EffectCoin.h"

void CEffectCoin::Update(DWORD dt, vector<LPGAMEOBJECT>*coObjects)
{
	y += vx * dt;

	if (y < highest)
		vy = -vy;

	if (vy > 0 && y > lowest)
		isDeleted = true;
}

void CEffectCoin::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_EFFECT_COIN)->Render(x, y);
}