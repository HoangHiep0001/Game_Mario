#include "Goomba.h"

CGoomba::CGoomba(float x, float y, int app):CGameObject(x, y)
{
	this->apperance = app;
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	die_start = -1;
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	switch (state)
	{
	case GOOMBA_STATE_WALKING:
		right = x + GOOMBA_BBOX_X_Y;
		bottom = y + GOOMBA_BBOX_X_Y;
		break;
	case GOOMBA_STATE_FLYLING:
		right = x + GOOMBA_BBOX_WING;
		bottom = y + GOOMBA_BBOX_FLYING_Y;
		break;
	case GOOMBA_STATE_WALKING_WING:
		right = x + GOOMBA_BBOX_WING;
		bottom = y + GOOMBA_BBOX_FLYING_Y;
		break;
	case GOOMBA_STATE_DIE:
		right = x + 0;
		bottom = y + 0;
		break;
	case GOOMBA_STATE_WALKING_DOWN:
	case GOOMBA_STATE_FLYLING_DOWN:
	case GOOMBA_STATE_WALKING_WING_DOWN:
		right = x + 0;
		bottom = y + 0;
		break;
	}

}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return; 
	if (dynamic_cast<CGoomba*>(e->obj)) return; 

	if (e->ny != 0 )
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ( (state==GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT) )
	{
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGoomba::Render()
{
	int ani = -1;
	if (apperance == GOOMBA_RED)
	{
		switch (state)
		{
		case GOOMBA_STATE_WALKING:
			ani = GOOMBA_ANI_RED_WALKING;
			break;
		case GOOMBA_STATE_FLYLING:
			ani = GOOMBA_ANI_RED_FLYLING;
			break;
		case GOOMBA_STATE_WALKING_WING:
			ani = GOOMBA_ANI_RED_WALKING_WING;
			break;
		case GOOMBA_STATE_DIE:
			ani = GOOMBA_ANI_RED_DIE;
			break;
		case GOOMBA_STATE_WALKING_DOWN:
			ani = GOOMBA_ANI_RED_WALKING_DOWN;
			break;
		case GOOMBA_STATE_FLYLING_DOWN:
			ani = GOOMBA_ANI_RED_FLYLING_DOWN;
			break;
		case GOOMBA_STATE_WALKING_WING_DOWN:
			ani = GOOMBA_ANI_RED_WALKING_WING_DOWN;
			break;
		}
	}
	else if (apperance == GOOMBA_THERE)
	{
		switch (state)
		{
		case GOOMBA_STATE_WALKING:
			ani = GOOMBA_ANI_THERE_WALKING;
			break;
		case GOOMBA_STATE_FLYLING:
			ani = GOOMBA_ANI_THERE_FLYLING;
			break;
		case GOOMBA_STATE_WALKING_WING:
			ani = GOOMBA_ANI_THERE_WALKING_WING;
			break;
		case GOOMBA_STATE_DIE:
			ani = GOOMBA_ANI_THERE_DIE;
			break;
		case GOOMBA_STATE_WALKING_DOWN:
			ani = GOOMBA_ANI_THERE_WALKING_DOWN;
			break;
		case GOOMBA_STATE_FLYLING_DOWN:
			ani = GOOMBA_ANI_THERE_FLYLING_DOWN;
			break;
		case GOOMBA_STATE_WALKING_WING_DOWN:
			ani = GOOMBA_ANI_THERE_WALKING_WING_DOWN;
			break;
		}
	}
	CAnimations::GetInstance()->Get(ani)->Render(x,y);
	RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{

	case GOOMBA_STATE_DIE:
		die_start = GetTickCount64();
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case GOOMBA_STATE_WALKING:
		if (nx > 0)
		{
			vx = GOOMBA_WALKING_SPEED;
		}
		else
		{
			vx = -GOOMBA_WALKING_SPEED;
		}
		break;
	case GOOMBA_STATE_FLYLING:
		if (nx > 0)
		{
			vx = GOOMBA_WALKING_SPEED;
		}
		else
		{
			vx = -GOOMBA_WALKING_SPEED;
		}
		vy = -GOOMBA_JUMP_FLY_SPEED_Y;
		break;
	case GOOMBA_STATE_WALKING_WING:
		if (nx > 0)
		{
			vx = GOOMBA_WALKING_SPEED;
		}
		else
		{
			vx = -GOOMBA_WALKING_SPEED;
		}
		vy = -GOOMBA_JUMP_SPEED_Y;
		break;
	case GOOMBA_STATE_WALKING_DOWN:
	case GOOMBA_STATE_FLYLING_DOWN:
	case GOOMBA_STATE_WALKING_WING_DOWN:
		vx = 0;
		vy = -GOOMBA_JUMP_SPEED_Y;
		break;
	}
	
}
