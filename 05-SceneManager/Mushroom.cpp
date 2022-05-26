#include "Mushroom.h"
#include "debug.h"
#include "Mario.h"

void CMushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - MUSHROOM_BBOX_WIDTH / 2;
	top = y - MUSHROOM_BBOX_HEIGHT / 2;
	right = left + MUSHROOM_BBOX_WIDTH;
	bottom = top + MUSHROOM_BBOX_HEIGHT;
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;

	if (y < highestPos)
	{
		y = highestPos;
		ay = MUSHROOM_GRAVITY;
		vx = -MUSHROOM_MOVING_SPEED;
	}

	float ml, mt, mr, mb, pl, pt, pr, pb;
	GetBoundingBox(ml, mt, mr, mb);
	CMario::GetInstance()->GetBoundingBox(pl, pt, pr, pb);
	if (CGameObject::CheckAABB(ml, mt, mr, mb, pl, pt, pr, pb))
	{
		CMario::GetInstance()->SetLevel(MARIO_LEVEL_BIG);
		isDeleted = true;
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMushroom::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_SUPER_MUSHROOM)->Render(x, y);

	//RenderBoundingBox();
}

void CMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		if (e->ny < 0) vy = 0;
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
	}
}
