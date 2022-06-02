#include "Koopa.h"
#include "debug.h"
#include "DetectionBox.h"
#include "Goomba.h"
#include "PandoraBrick.h"
#include "Mario.h"

#define GAME_SCREEN_WIDTH 320

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - KOOPA_BBOX_WIDTH / 2;
	right = left + KOOPA_BBOX_WIDTH;

	if (state == KOOPA_STATE_WALKING)
	{
		top = y - KOOPA_BBOX_HEIGHT / 2;
		bottom = top + KOOPA_BBOX_HEIGHT;
	}
	else
	{
		top = y - KOOPA_SHELL_OFFSET_TOP;
		bottom = top + KOOPA_SHELL_BBOX_HEIGHT;
	}
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (isBeingHeld) {
		SetPositionFollowPlayer();
	}

	if (shellTime->IsTimeUp()) {
		SetState(KOOPA_STATE_VIBRATE);
	}

	if (vibrationTime->IsTimeUp()) {
		SetState(KOOPA_STATE_WALKING);
	}

	if (state == -1 && CGame::GetInstance()->GetCamPosX() + GAME_SCREEN_WIDTH > x)
		SetState(KOOPA_STATE_WALKING);

	if (type == Type::RED_KOOPA)
		for (int i = 0; i < subItems.size(); i++)
			subItems[i]->Update(dt, coObjects);


	//CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopa::Render()
{
	int aniId = -1;

	if (type == Type::RED_KOOPA)
	{
		aniId = GetAniRed();
	}
	else
	{
		aniId = GetAniGreen();
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);

	RenderBoundingBox();

	if (type == Type::RED_KOOPA)
		for (int i = 0; i < subItems.size(); i++)
			subItems[i]->Render();
}

void CKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	//if (!e->obj->IsBlocking()) return;

	if (e->ny != 0)
	{
		if (e->ny < 0 && type == Type::GREEN_PARAKOOPA)
			vy = -PARAKOOPA_DEFLECT_SPEED_Y;
		else
			vy = 0;
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
		nx = -nx;
	}

	if (state != KOOPA_STATE_SHELL_MOVING) return;

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CPandoraBrick*>(e->obj))
		OnCollisionWithPandoraBrick(e);
}

void CKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	if (e->nx != 0)
	{
		e->obj->SetNx(nx);
		e->obj->SetState(GOOMBA_STATE_DIE_BY_ATTACK);
	}
}

void CKoopa::OnCollisionWithPandoraBrick(LPCOLLISIONEVENT e)
{
	if (e->nx != 0 && e->obj->GetState() != PANDORA_BRICK_STATE_ACTIVE)
		e->obj->SetState(PANDORA_BRICK_STATE_ACTIVE);
}

CKoopa::CKoopa(float x, float y, Type type) : CGameObject(x, y, type)
{
	isBeingHeld = false;
	isSupine = false;
	ax = 0.0f;
	ay = KOOPA_GRAVITY;
	//SetState(KOOPA_STATE_WALKING);
	CDetectionBox* detectionBox = new CDetectionBox(this);
	subItems.push_back(detectionBox);
}

void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case KOOPA_STATE_WALKING:
		isBeingHeld = false;
		vibrationTime->Stop();
		vx = -KOOPA_WALKING_SPEED;
		nx = -1;
		break;

	case KOOPA_STATE_SHELL:
		shellTime->Start();
		vx = 0;
		break;

	case KOOPA_STATE_SHELL_MOVING:
		shellTime->Stop();
		vibrationTime->Stop();
		vx = KOOPA_SHELL_MOVING_SPEED * nx;
		break;

	case KOOPA_STATE_BEING_HELD:
		isBeingHeld = true;
		break;

	case KOOPA_STATE_VIBRATE:
		shellTime->Stop();
		vibrationTime->Start();
		break;

	}
}

void CKoopa::ChangeDirection()
{
	if (state == KOOPA_STATE_WALKING)
	{
		vx = -vx;
		nx = -nx;
	}
}

void CKoopa::SetPositionFollowPlayer()
{
	float px, py; // x and y of player
	CMario::GetInstance()->GetPosition(px, py);

	if (CMario::GetInstance()->GetNx() > 0) {
		x = px + 10;
		y = py + 3;
	}
	else
	{
		x = px - 10;
		y = py + 3;
	}
}

int CKoopa::GetAniRed()
{
	int aniId = ID_ANI_RED_KOOPA_WALKING_LEFT;

	if (state == KOOPA_STATE_SHELL)
	{
		if (isSupine)
			aniId = ID_ANI_RED_KOOPA_SHELL_SUPINE;
		else
			aniId = ID_ANI_RED_KOOPA_SHELL_PRONE;
	}
	else if (state == KOOPA_STATE_SHELL_MOVING)
	{
		if (isSupine)
			aniId = ID_ANI_RED_KOOPA_SHELL_MOVING_SUPINE;
		else
			aniId = ID_ANI_RED_KOOPA_SHELL_MOVING_PRONE;
	}
	else if (state == KOOPA_STATE_VIBRATE)
	{
		if (isSupine)
			aniId = ID_ANI_RED_KOOPA_VIBRATE_SUPINE;
		else
			aniId = ID_ANI_RED_KOOPA_VIBRATE_PRONE;
	}
	else
	{
		if (nx > 0)
			aniId = ID_ANI_RED_KOOPA_WALKING_RIGHT;
		else
			aniId = ID_ANI_RED_KOOPA_WALKING_LEFT;
	}

	return aniId;
}

int CKoopa::GetAniGreen()
{
	int aniId = ID_ANI_GREEN_KOOPA_WALKING_LEFT;

	if (state == KOOPA_STATE_SHELL)
	{
		if (isSupine)
			aniId = ID_ANI_GREEN_KOOPA_SHELL_SUPINE;
		else
			aniId = ID_ANI_GREEN_KOOPA_SHELL_PRONE;
	}
	else if (state == KOOPA_STATE_SHELL_MOVING)
	{
		if (isSupine)
			aniId = ID_ANI_GREEN_KOOPA_SHELL_MOVING_SUPINE;
		else
			aniId = ID_ANI_GREEN_KOOPA_SHELL_MOVING_PRONE;
	}
	else if (state == KOOPA_STATE_VIBRATE)
	{
		if (isSupine)
			aniId = ID_ANI_GREEN_KOOPA_VIBRATE_SUPINE;
		else
			aniId = ID_ANI_GREEN_KOOPA_VIBRATE_PRONE;
	}
	else
	{
		if (type == Type::GREEN_PARAKOOPA)
		{
			if (nx > 0)
				aniId = ID_ANI_GREEN_PARAKOOPA_WALKING_RIGHT;
			else
				aniId = ID_ANI_GREEN_PARAKOOPA_WALKING_LEFT;
		}
		else
		{
			if (nx > 0)
				aniId = ID_ANI_GREEN_KOOPA_WALKING_RIGHT;
			else
				aniId = ID_ANI_GREEN_KOOPA_WALKING_LEFT;
		}
	}

	return aniId;
}
