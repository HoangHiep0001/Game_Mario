#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"

#include "Collision.h"
#include "Ground.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;
	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	isOnPlatform = false;

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else 
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<Ground*>(e->obj))
		OnCollisionWithGround(e);
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::OnCollisionWithGround(LPCOLLISIONEVENT e)
{
	Ground* ground = dynamic_cast<Ground*>(e->obj);
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
	if (e->nx != 0)
	{
		if (dynamic_cast<Ground*>(e->obj)->GetGroundState() == 1)
		{
			x += dx;
		}
		else if (dynamic_cast<Ground*>(e->obj)->GetGroundState() == 0)
		{
			vx = 0;

		}
	}

	/*vector<LPCOLLISIONEVENT> coEvents;
	vector<LPGAMEOBJECT>* coObjects;	//cai nay lay tren ham update
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	bool flagOnGround = false;
	for (std::size_t i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT e = coObjects->at(i);
		if (dynamic_cast<Ground*>(e) && !flagOnGround)
		{
			Ground* f = dynamic_cast<Ground*> (e);

			float l, t, r, b, el, et, er, eb;
			this->GetBoundingBox(l, t, r, b);
			b = b;
			f->GetBoundingBox(el, et, er, eb);
			if (CGameObject::AABB(l, t, r, b, el, et, er, eb))
			{
				if (b > et && state != MARIO_STATE_JUMP && state != MARIO_STATE_FLYLING)
				{

					flagOnGround = true;
				}
			}
		}
	}*/
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int ani = -1;
	if (level == MARIO_LEVEL_SMALL)
	{
		switch (state)
		{
		case MARIO_STATE_FALL_DOWN:
			if (nx > 0)
				ani = MARIO_ANI_SMALL_FALL_DOWN_RIGHT;
			else
				ani = MARIO_ANI_SMALL_FALL_DOWN_LEFT;
			break;
		case MARIO_STATE_IDLE:
			if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
			else ani = MARIO_ANI_SMALL_IDLE_LEFT;
			break;
		case MARIO_STATE_WALKING_RIGHT:
			ani = MARIO_ANI_SMALL_WALKING_RIGHT;
			break;
		case MARIO_STATE_WALKING_RIGHT_FAST:
			ani = MARIO_ANI_SMALL_WALKING_FAST_RIGHT;
			break;
		case MARIO_STATE_WALKING_LEFT:
			ani = MARIO_ANI_SMALL_WALKING_LEFT;
			break;
		case MARIO_STATE_WALKING_LEFT_FAST:
			ani = MARIO_ANI_SMALL_WALKING_FAST_LEFT;
			break;
		case MARIO_STATE_JUMP:
		case MARIO_STATE_JUMP_MAX:
			if (nx > 0)
				ani = MARIO_ANI_SMALL_JUMP_IDLE_RIGHT;
			else
				ani = MARIO_ANI_SMALL_JUMP_IDLE_LEFT;
			break;

		default:
			if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
			else ani = MARIO_ANI_SMALL_IDLE_LEFT;
			break;
		case MARIO_STATE_STOP:
			if (nx < 0)
				ani = MARIO_ANI_SMALL_STOP_IDLE_RIGHT;
			else
				ani = MARIO_ANI_SMALL_STOP_IDLE_LEFT;
			break;
		case MARIO_STATE_FLY:
			if (nx > 0)
				ani = MARIO_ANI_SMALL_FLY_IDLE_RIGHT;
			else
				ani = MARIO_ANI_SMALL_FLY_IDLE_LEFT;
			break;
		case MARIO_STATE_HOLD:
			if (nx > 0)
				ani = MARIO_ANI_SMALL_HOLD_RIGHT;
			else
				ani = MARIO_ANI_SMALL_HOLD_LEFT;
			break;
		case MARIO_STATE_STONE_KOOPAS:
			if (nx > 0)
				ani = MARIO_ANI_SMALL_STONE_KOOPAS_RIGHT;
			else
				ani = MARIO_ANI_SMALL_STONE_KOOPAS_LEFT;
			break;
		
		}
	}

	return ani;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int ani = -1;
	if (level == MARIO_LEVEL_BIG)
	{
		switch (state)
		{
		case MARIO_STATE_IDLE:
			if (nx > 0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
			else ani = MARIO_ANI_BIG_IDLE_LEFT;
			break;
		case MARIO_STATE_WALKING_RIGHT:
			ani = MARIO_ANI_BIG_WALKING_RIGHT;
			break;
		case MARIO_STATE_WALKING_RIGHT_FAST:
			ani = MARIO_ANI_BIG_WALKING_FAST_RIGHT;
			break;
		case MARIO_STATE_WALKING_LEFT:
			ani = MARIO_ANI_BIG_WALKING_LEFT;
			break;
		case MARIO_STATE_WALKING_LEFT_FAST:
			ani = MARIO_ANI_BIG_WALKING_FAST_LEFT;
			break;
		case MARIO_STATE_JUMP:
		case MARIO_STATE_JUMP_MAX:
			if (nx > 0)
				ani = MARIO_ANI_BIG_JUMP_IDLE_RIGHT;
			else
				ani = MARIO_ANI_BIG_JUMP_IDLE_LEFT;
			break;
		case MARIO_STATE_FALL_DOWN:
			if (nx > 0)
				ani = MARIO_ANI_BIG_FALL_DOWN_RIGHT;
			else
				ani = MARIO_ANI_BIG_FALL_DOWN_LEFT;
			break;
		case MARIO_STATE_SIT:
			if (nx > 0) ani = MARIO_ANI_BIG_SIT_RIGHT;
			else ani = MARIO_ANI_BIG_SIT_LEFT;
			break;
		default:
			if (nx > 0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
			else ani = MARIO_ANI_BIG_IDLE_LEFT;
			break;
		case MARIO_STATE_STOP:
			if (nx < 0)
				ani = MARIO_ANI_BIG_STOP_IDLE_RIGHT;
			else
				ani = MARIO_ANI_BIG_STOP_IDLE_LEFT;
			break;
		case MARIO_STATE_FLY:
			if (nx > 0)
				ani = MARIO_ANI_BIG_FLY_IDLE_RIGHT;
			else
				ani = MARIO_ANI_BIG_FLY_IDLE_LEFT;
			break;
		case MARIO_STATE_HOLD:
			if (nx > 0)
				ani = MARIO_ANI_BIG_HOLD_RIGHT;
			else
				ani = MARIO_ANI_BIG_HOLD_LEFT;
			break;
		case MARIO_STATE_STONE_KOOPAS:
			if (nx > 0)
				ani = MARIO_ANI_BIG_STONE_KOOPAS_RIGHT;
			else
				ani = MARIO_ANI_BIG_STONE_KOOPAS_LEFT;
			break;

		}
	}
	return ani;
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();

	animations->Get(aniId)->Render(x, y);

	RenderBoundingBox();
	
	DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return; 

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT_FAST:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT_FAST:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	/*case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;*/

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y +=MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	/*case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;*/

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level==MARIO_LEVEL_BIG)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else 
		{
			left = x - MARIO_BIG_BBOX_WIDTH/5;
			top = y - MARIO_BIG_BBOX_HEIGHT/5;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x ;
		top = y ;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}

