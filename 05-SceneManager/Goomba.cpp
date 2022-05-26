#include "Goomba.h"
#include "Mario.h"

#define GAME_SCREEN_WIDTH 320

CGoomba::CGoomba(float x, float y, Type type) : CGameObject(x, y, type)
{
	lowFlyingCounter = 0;
	lostWings = false;
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
}

float CGoomba::GetSpeedX()
{
	if (CMario::GetInstance()->GetPosX() > this->x)
		return GOOMBA_WALKING_SPEED;
	else
		return -GOOMBA_WALKING_SPEED;
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == GOOMBA_STATE_DIE_BY_ATTACK) return;

	if (state == GOOMBA_STATE_DIE_BY_CRUSH)
	{
		if (type == Type::YELLOW_GOOMBA)
			top = y - GOOMBA_DIE_OFFSET_TOP;
		else
			top = y + GOOMBA_DIE_OFFSET_TOP;

		left = x - GOOMBA_DIE_OFFSET_LEFT;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_DIE_BBOX_HEIGHT;
	}
	else if (type == Type::YELLOW_GOOMBA)
	{ 
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y - GOOMBA_WALKING_OFFSET_TOP;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT;
	}
	else
	{
		left = x - PARAGOOMBA_BBOX_WIDTH / 2;
		top = y - PARAGOOMBA_DIE_OFFSET_TOP;
		right = left + PARAGOOMBA_BBOX_WIDTH;
		bottom = top + PARAGOOMBA_BBOX_HEIGHT;
	}
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	/*if (!e->obj->IsBlocking()) return; 
	if (dynamic_cast<CGoomba*>(e->obj)) return; */

	if (e->ny != 0 )
	{
		vy = 0;

		//if (state == GOOMBA_STATE_DIE_BY_ATTACK || lostWings || type == Type::YELLOW_GOOMBA) return;

		if (state == PARAGOOMBA_STATE_FLY_LOW && lowFlyingCounter < 3)
			SetState(PARAGOOMBA_STATE_FLY_LOW);
		else if (state == PARAGOOMBA_STATE_FLY_LOW && lowFlyingCounter == 3)
			SetState(PARAGOOMBA_STATE_FLY_HIGH);
		else if (state == PARAGOOMBA_STATE_FLY_HIGH)
			SetState(GOOMBA_STATE_WALKING);
	}
	else if (e->nx != 0)
	{
		vx = -vx;	
		if (!e->obj->IsBlocking())
			e->obj->ChangeDirection();
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (state == PARAGOOMBA_STATE_FLY_HIGH)
		ay = PARAGOOMBA_HIGH_FLYING_GRAVITY;
	else
		ay = GOOMBA_GRAVITY;

	vy += ay * dt;
	vx += ax * dt;

	if (state == GOOMBA_STATE_DIE_BY_CRUSH && dieTime->IsTimeUp())
	{
		isDeleted = true;
		return;
	}

	if (state == -1 && CGame::GetInstance()->GetCamPosX() + GAME_SCREEN_WIDTH > x)
	{
		SetState(GOOMBA_STATE_WALKING);

		if (type == Type::RED_PARAGOOMBA)
			chasingTime->Start();
	}

	if (type == Type::RED_PARAGOOMBA && state == GOOMBA_STATE_WALKING && redirectionDelay->IsTimeUp() && !lostWings && !chasingTime->IsTimeUp())
	{
		vx = GetSpeedX();
		redirectionDelay->Stop();
	}

	if (walkTime->IsTimeUp())
	{
		SetState(PARAGOOMBA_STATE_FLY_LOW);
		walkTime->Stop();
	}

	//CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGoomba::Render()
{
	int aniId = ID_ANI_GOOMBA_WALKING;

	if (type == Type::RED_PARAGOOMBA)
	{
		if (state == GOOMBA_STATE_DIE_BY_CRUSH)
			aniId = ID_ANI_PARAGOOMBA_DIE_BY_CRUSH;
		else if (state == GOOMBA_STATE_DIE_BY_ATTACK)
			aniId = ID_ANI_PARAGOOMBA_DIE_BY_ATTACK;
		else if (state == PARAGOOMBA_STATE_FLY_LOW)
			aniId = ID_ANI_PARAGOOMBA_FLAP_WINGS_SLOWLY;
		else if (state == PARAGOOMBA_STATE_FLY_HIGH)
			aniId = ID_ANI_PARAGOOMBA_FLAP_WINGS_QUICKLY;
		else if (state == PARAGOOMBA_STATE_NORMAL)
			aniId = ID_ANI_PARAGOOMBA_NORMAL_WALKING;
		else
			aniId = ID_ANI_PARAGOOMBA_WINGS_WALKING;
	}
	else
	{
		if (state == GOOMBA_STATE_DIE_BY_CRUSH)
			aniId = ID_ANI_GOOMBA_DIE_BY_CRUSH;
		else if (state == GOOMBA_STATE_DIE_BY_ATTACK)
			aniId = ID_ANI_GOOMBA_DIE_BY_ATTACK;
		else
			aniId = ID_ANI_GOOMBA_WALKING;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x,y);

	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE_BY_ATTACK:
		walkTime->Stop();
		vx = vx * nx;
		vy = -GOOMBA_DIE_DEFLECT_SPEED_Y;
		break;

	case GOOMBA_STATE_DIE_BY_CRUSH:
		dieTime->Start();
		vx = 0;
		vy = 0;
		ay = 0; 
		break;

	case GOOMBA_STATE_WALKING: 
		if (type == Type::RED_PARAGOOMBA)
		{
			if (!chasingTime->IsTimeUp())
				vx = GetSpeedX();
			if (!lostWings)
			{
				walkTime->Start();
				redirectionDelay->Start();
			}
		}
		else
			vx = -GOOMBA_WALKING_SPEED;
		break;

	case PARAGOOMBA_STATE_NORMAL:
		lostWings = true;
		walkTime->Stop();
		break;

	case PARAGOOMBA_STATE_FLY_LOW:
		vy = -PARAGOOMBA_LOW_FLYING_SPEED_Y;
		lowFlyingCounter++;
		break;

	case PARAGOOMBA_STATE_FLY_HIGH:
		lowFlyingCounter = 0;
		vy = -PARAGOOMBA_HIGH_FLYING_SPEED_Y;
		break;
	}
}
