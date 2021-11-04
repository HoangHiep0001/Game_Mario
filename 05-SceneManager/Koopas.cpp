#include "Koopas.h"
#include "Ground.h"

CKoopas::CKoopas(float x, float y, int app) :CGameObject(x, y)
{
	this->apperance = app;
	this->ax = 0;
	this->vy = KOOPAS_GRAVITY;

	SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	switch (state)
	{
	case KOOPAS_STATE_WALKING:
		right = x + KOOPAS_BBOX_WIDTH;
		bottom = y + KOOPAS_BBOX_HEIGHT;
		break;
	case KOOPAS_STATE_FLYLING:
		right = x + KOOPAS_BBOX_WIDTH;
		bottom = y + KOOPAS_BBOX_HEIGHT;
		break;
	case KOOPAS_STATE_DIE_UP:
		right = x;
		bottom = y;
		break;
	case KOOPAS_STATE_LIVING_UP:
		right = x + KOOPAS_BBOX_LIVING;
		bottom = y + KOOPAS_BBOX_LIVING;
		break;
	case KOOPAS_STATE_TORTOISESHELL_UP:
		right = x + KOOPAS_BBOX_LIVING;
		bottom = y + KOOPAS_BBOX_LIVING;
		break;
	case KOOPAS_STATE_DIE_DOWN:
		right = x;
		bottom = y;
		break;
	case KOOPAS_STATE_LIVING_DOWN:
		right = x + KOOPAS_BBOX_LIVING;
		bottom = y + KOOPAS_BBOX_LIVING;
		break;
	case KOOPAS_STATE_TORTOISESHELL_DOWN:
		right = x + KOOPAS_BBOX_LIVING;
		bottom = y + KOOPAS_BBOX_LIVING;
		break;
	case KOOPAS_STATE_LIVE_FOOT_DOWN:
		break;
	case KOOPAS_STATE_LIVE_FOOT_UP:
		right = x + KOOPAS_BBOX_LIVING;
		bottom = y + KOOPAS_BBOX_LIVING;
		break;
	}

}

void CKoopas::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopas::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	//if (dynamic_cast<CKoopas*>(e->obj)) return;
	if (dynamic_cast<Ground*>(e->obj))
	{
		if (e->ny < 0)
		{
			if (dynamic_cast<Ground*>(e->obj)->GetGroundState() == 1)
			{
				y += dy;
			}

		}
		else
		{
			vy = 0;

		}
		if (dynamic_cast<Ground*>(e->obj)->GetGroundState() == 0)
		{
			if (e->ny > 0)
			{
				vy = 0;
			}
			if (e->nx != 0)
			{
				nx = -nx;
				vx = -vx;
			}
		}
		if (state == KOOPAS_STATE_FLYLING)
		{
			vy = -KOOPAS_JUMP_SPEED_Y;
		}
	}
	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;	//cai nay la roi theo trong luc
	vx += ax * dt;

	if ((state == KOOPAS_STATE_DIE_UP) && (GetTickCount64() - die_start > KOOPAS_TIME_DIE))
	{
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CKoopas::Render()
{
	int ani = -1;
	if (apperance == KOOPAS_RED)
	{
		switch (state)
		{
		case KOOPAS_STATE_WALKING:
			if (vx > 0)
			{
				ani = KOOPAS_ANI_RED_WALKING_RIGHT;
			}
			else
			{
				ani = KOOPAS_ANI_RED_WALKING_LEFT;
			}

			break;
		case KOOPAS_STATE_FLYLING:
			if (vx > 0)
			{
				ani = KOOPAS_ANI_RED_FLYLING_RIGHT;
			}
			else
			{
				ani = KOOPAS_ANI_RED_FLYLING_LEFT;
			}
			break;
		case KOOPAS_STATE_DIE_UP:
		case KOOPAS_STATE_LIVING_UP:
			ani = KOOPAS_ANI_RED_DIE_UP;
			break;
		case KOOPAS_STATE_TORTOISESHELL_UP:
			ani = KOOPAS_ANI_RED_TORTOISESHELL_UP;
			break;
		case KOOPAS_STATE_DIE_DOWN:
		case KOOPAS_STATE_LIVING_DOWN:
			ani = KOOPAS_ANI_RED_DIE_DOWN;
			break;
		case KOOPAS_STATE_TORTOISESHELL_DOWN:
			ani = KOOPAS_ANI_RED_TORTOISESHELL_DOWN;
			break;
		case KOOPAS_STATE_LIVE_FOOT_UP:
			ani = KOOPAS_ANI_RED_LIVE_FOOT_UP;
			break;
		case KOOPAS_STATE_LIVE_FOOT_DOWN:
			ani = KOOPAS_ANI_RED_LIVE_FOOT_DOWN;
			break;
		}
	}
	else if (apperance == KOOPAS_BULE)
	{
		switch (state)
		{
		case KOOPAS_STATE_WALKING:
			if (vx > 0)
			{
				ani = KOOPAS_ANI_BULE_WALKING_RIGHT;
			}
			else
			{
				ani = KOOPAS_ANI_BULE_WALKING_LEFT;
			}
			break;
		case KOOPAS_STATE_FLYLING:
			if (vx > 0)
			{
				ani = KOOPAS_ANI_BULE_FLYLING_RIGHT;
			}
			else
			{
				ani = KOOPAS_ANI_BULE_FLYLING_LEFT;
			}
			break;
		case KOOPAS_STATE_DIE_UP:
		case KOOPAS_STATE_LIVING_UP:
			ani = KOOPAS_ANI_BULE_DIE_UP;
			break;
		case KOOPAS_STATE_TORTOISESHELL_UP:
			ani = KOOPAS_ANI_BULE_TORTOISESHELL_UP;
			break;
		case KOOPAS_STATE_DIE_DOWN:
		case KOOPAS_STATE_LIVING_DOWN:
			ani = KOOPAS_ANI_BULE_DIE_DOWN;
			break;
		case KOOPAS_STATE_TORTOISESHELL_DOWN:
			ani = KOOPAS_ANI_BULE_TORTOISESHELL_DOWN;
			break;
		case KOOPAS_STATE_LIVE_FOOT_UP:
			ani = KOOPAS_ANI_BULE_LIVE_FOOT_UP;
			break;
		case KOOPAS_STATE_LIVE_FOOT_DOWN:
			ani = KOOPAS_ANI_BULE_LIVE_FOOT_DOWN;
			break;
		}
	}
	CAnimations::GetInstance()->Get(ani)->Render(x, y);
	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE_UP:
		vx = 0;
		vy = 0;
		ax = 0;
		ay = 0;
		break;
	case KOOPAS_STATE_LIVING_UP:
		vx = 0;
		vy = 0;
		time = GetTickCount64();
		break;
	case KOOPAS_STATE_DIE_DOWN:
		vx = 0;
		vy = -KOOPAS_DIE_VY;
		time = GetTickCount64();
		break;
	case KOOPAS_STATE_LIVING_DOWN:
		vx = 0;
		time = GetTickCount64();
		break;
	case KOOPAS_STATE_WALKING:
		nx = -1;
		if (nx > 0)
		{
			vx = KOOPAS_WALKING_SPEED;
		}
		else
		{
			vx = -KOOPAS_WALKING_SPEED;
		}
		break;
	case KOOPAS_STATE_FLYLING:
		nx = -1;
		vy = -KOOPAS_JUMP_SPEED_Y;
		vx = -KOOPAS_WALKING_SPEED;
		break;
	case KOOPAS_STATE_TORTOISESHELL_UP:
		if (nx > 0)
		{
			vx = KOOPAS_TORTOISESHELL;
		}
		else
		{
			vx = -KOOPAS_TORTOISESHELL;
		}
		time = GetTickCount64();
		break;
	case KOOPAS_STATE_TORTOISESHELL_DOWN:
		if (nx > 0)
		{
			vx = KOOPAS_TORTOISESHELL;
		}
		else
		{
			vx = -KOOPAS_TORTOISESHELL;
		}
		time = GetTickCount64();
		break;
	case KOOPAS_STATE_LIVE_FOOT_UP:
	case KOOPAS_STATE_LIVE_FOOT_DOWN:
		break;
	}
}
