#include "EffectTail.h"

void CEffectTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (effectTime->IsTimeUp())
		isDeleted = true;
}

void CEffectTail::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_TAIL_HIT_EFFECT)->Render(x, y);
}
