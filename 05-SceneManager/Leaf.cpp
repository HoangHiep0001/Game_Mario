#include "Leaf.h"
#include "Mario.h"

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += vx;
	y += vy;

	if (y < highest)
		vy = LEAF_SPEED_Y;

	if (vy > 0)
	{
		if (x <= leftLeaf)
			vx = (float)(LEAF_FACTOR * pow(LEAF_POW, vy));

		if (x >= rightLeaf)
			vx = (float)(-LEAF_FACTOR * pow(LEAF_POW, vy));
	}

	float ml, mt, mr, mb, pl, pt, pr, pb;
	GetBoundingBox(ml, mt, mr, mb);
	CMario::GetInstance()->GetBoundingBox(pl, pt, pr, pb);
	if (CGameObject::CheckAABB(ml, mt, mr, mb, pl, pt, pr, pb))
	{
		CMario::GetInstance()->SetLevel(MARIO_LEVEL_RACCOON);
		isDeleted = true;
	}
}

void CLeaf::Render()
{
	CAnimations* animations = CAnimations::GetInstance();

	if (vx > 0)
		animations->Get(ID_ANI_LEAF_FALLING_RIGHT)->Render(x, y);
	else
		animations->Get(ID_ANI_LEAF_FALLING_LEFT)->Render(x, y);

	RenderBoundingBox();
}

void CLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - LEAF_BBOX_WIDTH / 2;
	top = y - LEAF_BBOX_HEIGHT / 2;
	right = left + LEAF_BBOX_WIDTH;
	bottom = top + LEAF_BBOX_HEIGHT;
}