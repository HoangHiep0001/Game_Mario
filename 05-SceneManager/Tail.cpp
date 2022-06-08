#include "Tail.h"
#include "Mario.h"
#include "Koopa.h"
#include "Goomba.h"
#include "MagicCoinBrick.h"
#include "PandoraBrick.h"

void CTail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!attackIsOn) return;
	left = x;
	top = y;
	right = left + TAIL_BBOX_WIDTH;
	bottom = top + TAIL_BBOX_HEIGHT;
}

void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float px, py;
	CMario::GetInstance()->GetPosition(px, py);

	if (attackTime->IsTimeUp() || attackTime->IsStopped()) {
		attackIsOn = false;
		return;
	}

	if (attackTime->GetElapsedTime() >= 200)
	{
		if (CMario::GetInstance()->GetNx() > 0)
			SetPosition(px - TAIL_OFFSET_X_LEFT, py + TAIL_OFFSET_Y);
		else
			SetPosition(px + TAIL_OFFSET_X_RIGHT, py + TAIL_OFFSET_Y);

		attackIsOn = true;
	}
	else if (attackTime->GetElapsedTime() >= 150)
	{
		attackIsOn = false;
	}
	else if (attackTime->GetElapsedTime() >= 100)
	{
		if (CMario::GetInstance()->GetNx() > 0)
			SetPosition(px + TAIL_OFFSET_X_RIGHT, py + TAIL_OFFSET_Y);
		else
			SetPosition(px - TAIL_OFFSET_X_LEFT, py + TAIL_OFFSET_Y);

		attackIsOn = true;
	}
	else if (attackTime->GetElapsedTime() >= 50)
	{
		attackIsOn = false;
	}

	float ml, mt, mr, mb, sl, st, sr, sb; // main object (m) and scene objects (b)
	GetBoundingBox(ml, mt, mr, mb);

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT e = coObjects->at(i);
		e->GetBoundingBox(sl, st, sr, sb);
		if (CGameObject::CheckAABB(ml, mt, mr, mb, sl, st, sr, sb))
		{
			if (e->GetType() == Type::PANDORA_BRICK)
				OnCollisionWithPandoraBrick(e);
			else if (dynamic_cast<CGoomba*>(e))
				OnCollisionWithGoomba(e);
			else if (dynamic_cast<CKoopa*>(e))
				OnCollisionWithKoopa(e);
			else if (dynamic_cast<CMagicCoinBrick*>(e))
				OnCollisionWithMagicCoinBrick(e);
		}
	}
}

void CTail::Render()
{
	RenderBoundingBox();
}

void CTail::Attack()
{
	float px, py;
	CMario::GetInstance()->GetPosition(px, py);
	if (CMario::GetInstance()->GetNx() > 0)
		SetPosition(px - TAIL_OFFSET_X_LEFT, py + TAIL_OFFSET_Y);
	else
		SetPosition(px + TAIL_OFFSET_X_RIGHT, py + TAIL_OFFSET_Y);

	attackIsOn = true;
}

void CTail::OnCollisionWithGoomba(LPGAMEOBJECT e)
{
	if (CMario::GetInstance()->GetPosX() > e->GetPosX())
		e->SetNx(-TAIL_SETNX);
	else
		e->SetNx(TAIL_SETNX);

	e->SetState(GOOMBA_STATE_DIE_BY_ATTACK);
}

void CTail::OnCollisionWithKoopa(LPGAMEOBJECT e)
{
	if (CMario::GetInstance()->GetPosX() > e->GetPosX())
		e->SetNx(TAIL_SETNX);
	else
		e->SetNx(TAIL_SETNX);
	
	e->SetState(KOOPA_STATE_SHELL_BY_ATTACK);
}

void CTail::OnCollisionWithPandoraBrick(LPGAMEOBJECT e)
{
	if (e->GetState() != PANDORA_BRICK_STATE_ACTIVE)
		e->SetState(PANDORA_BRICK_STATE_ACTIVE);
}

void CTail::OnCollisionWithMagicCoinBrick(LPGAMEOBJECT e)
{
	if (e->GetState() != MAGIC_BRICK_STATE_BREAK)
		e->SetState(MAGIC_BRICK_STATE_BREAK);
}
