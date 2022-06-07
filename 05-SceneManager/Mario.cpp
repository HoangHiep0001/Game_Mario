#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Koopa.h"
#include "PandoraBrick.h"
#include "Portal.h"

#include "Collision.h"
#include "Textures.h"

CMario* CMario::__instance = nullptr;

CMario* CMario::GetInstance()
{
	if (__instance == NULL) __instance = new CMario(0.0f, 0.0f, Type::MARIO);
	return __instance;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (!powerMode->IsTimeUp() && !powerMode->IsStopped() && !isOnPlatform)
		ay = 0.0003f;
	else if (!wagTail->IsStopped() && !wagTail->IsTimeUp())
		ay = 0.000035f;
	else
		ay = MARIO_GRAVITY;

	vy += ay * dt;
	vx += ax * dt;

	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	if (kickShell->IsTimeUp()) 
		kickShell->Stop();

	if (spinTail->IsTimeUp())
		spinTail->Stop();

	if (wagTail->IsTimeUp())
		wagTail->Stop();

	if (powerMode->IsTimeUp())
		powerMode->Stop();

	isOnPlatform = false;

	tail->Update(dt, coObjects);

	dax = MARIO_DECEL_X * dt;

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
		if (e->ny < 0)
		{
			wagTail->Stop();
			isOnPlatform = true;
		}
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	else if (e->obj->GetType() == Type::COIN)
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPandoraBrick*>(e->obj))
		OnCollisionWithPandoraBrick(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
}

void CMario::DecelerateSlightly()
{
	ax = 0.0f;

	if (vx > 0)
	{
		vx -= dax;
		if (vx < 0)
			vx = 0;
	}
	else
	{
		vx += dax;
		if (vx > 0)
			vx = 0;
	}
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE_BY_CRUSH)
		{
			if (goomba->GetType() == Type::RED_PARAGOOMBA && goomba->HasWings())
				goomba->SetState(PARAGOOMBA_STATE_NORMAL);
			else
				goomba->SetState(GOOMBA_STATE_DIE_BY_CRUSH);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE_BY_CRUSH)
			{
				if (level > MARIO_LEVEL_BIG)
				{
					level = MARIO_LEVEL_BIG;
					StartUntouchable();
				}
				else if (level > MARIO_LEVEL_SMALL)
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

void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (koopa->GetState() == KOOPA_STATE_WALKING)
		{
			if (koopa->GetType() == Type::GREEN_PARAKOOPA)
				koopa->SetType(Type::GREEN_KOOPA);
			else
				koopa->SetState(KOOPA_STATE_SHELL);

			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (koopa->GetState() == KOOPA_STATE_SHELL_MOVING)
		{
			koopa->SetState(KOOPA_STATE_SHELL);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (koopa->GetState() == KOOPA_STATE_SHELL)
		{
			koopa->SetNx(this->nx);
			koopa->SetState(KOOPA_STATE_SHELL_MOVING);
		}
	}
	else if (e->nx != 0 && koopa->GetState() == KOOPA_STATE_SHELL)
	{
		if (abs(maxVx) == MARIO_MAX_RUNNING_SPEED)
		{
			isHoldingShell = true;
			koopa->SetNx(this->nx);
			koopa->SetState(KOOPA_STATE_BEING_HELD);
		}
		else
		{
			kickShell->Start();
			koopa->SetNx(this->nx);
			koopa->SetState(KOOPA_STATE_SHELL_MOVING);
		}
	}
	else // hit by Koopa
	{
		if (untouchable == 0)
		{
			if (koopa->GetState() != KOOPA_STATE_SHELL)
			{
				if (level > MARIO_LEVEL_BIG)
				{
					level = MARIO_LEVEL_BIG;
					StartUntouchable();
				}
				else if (level > MARIO_LEVEL_SMALL)
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

void CMario::OnCollisionWithPandoraBrick(LPCOLLISIONEVENT e)
{
	if (e->ny > 0 && e->obj->GetState() != PANDORA_BRICK_STATE_ACTIVE)
		e->obj->SetState(PANDORA_BRICK_STATE_ACTIVE);
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(vx) == MARIO_MAX_RUNNING_SPEED)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else if (isHoldingShell)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_HOLD_SHELL_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_HOLD_SHELL_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (!(kickShell->IsTimeUp() || kickShell->IsStopped()))
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_KICK_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_KICK_LEFT;
			}
			else if (vx == 0)
			{
				if (isHoldingShell)
				{
					if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_HOLD_SHELL_RIGHT;
					else aniId = ID_ANI_MARIO_SMALL_IDLE_HOLD_SHELL_LEFT;
				}
				else
				{
					if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
					else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
				}
			}
			else if (vx > 0)
			{
				if (isHoldingShell)
				{
					if (ax < 0)
						aniId = ID_ANI_MARIO_SMALL_WALK_HOLD_SHELL_LEFT;
					else
						aniId = ID_ANI_MARIO_SMALL_WALK_HOLD_SHELL_RIGHT;
				}
				else if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_STOP_LEFT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else/* if (ax == MARIO_ACCEL_WALK_X)*/
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (isHoldingShell)
				{
					if (ax > 0)
						aniId = ID_ANI_MARIO_SMALL_WALK_HOLD_SHELL_RIGHT;
					else
						aniId = ID_ANI_MARIO_SMALL_WALK_HOLD_SHELL_LEFT;
				}
				else if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_STOP_RIGHT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else/* if (ax == -MARIO_ACCEL_WALK_X)*/
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}

int CMario::GetAniIdRaccoon()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(vx) == MARIO_MAX_RUNNING_SPEED)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RACCOON_FLYING_UP_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_FLYING_UP_LEFT;
		}
		else
		{
			if (isSitting)
			{
				if (nx > 0)
					aniId = ID_ANI_MARIO_RACCOON_SIT_RIGHT;
				else
					aniId = ID_ANI_MARIO_RACCOON_SIT_LEFT;
			}
			else if (!(powerMode->IsTimeUp() || powerMode->IsStopped()))
			{
				if (nx > 0)
					aniId = ID_ANI_MARIO_RACCOON_FLY_WAG_TAIL_RIGHT;
				else
					aniId = ID_ANI_MARIO_RACCOON_FLY_WAG_TAIL_LEFT;
			}
			else if (!(wagTail->IsTimeUp() || wagTail->IsStopped()))
			{
				if (nx > 0)
					aniId = ID_ANI_MARIO_RACCOON_JUMP_WAG_TAIL_RIGHT;
				else
					aniId = ID_ANI_MARIO_RACCOON_JUMP_WAG_TAIL_LEFT;
			}
			else if (isHoldingShell)
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_RACCOON_FLY_HOLD_SHELL_RIGHT;
				else
					aniId = ID_ANI_MARIO_RACCOON_FLY_HOLD_SHELL_LEFT;
			}
			else if (vy > 0)
			{
				if (nx > 0)
					aniId = ID_ANI_MARIO_RACCOON_FALLING_RIGHT;
				else
					aniId = ID_ANI_MARIO_RACCOON_FALLING_LEFT;
			}
			else // vy < 0
			{
				if (nx > 0)
					aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT;
				else
					aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT;
			}
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_RACCOON_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_SIT_LEFT;
		}
		else
			if (!(kickShell->IsTimeUp() || kickShell->IsStopped()))
			{
				if (nx > 0) aniId = ID_ANI_MARIO_RACCOON_KICK_RIGHT;
				else aniId = ID_ANI_MARIO_RACCOON_KICK_LEFT;
			}
			else if (!(spinTail->IsTimeUp() || spinTail->IsStopped()))
			{
				if (nx > 0) aniId = ID_ANI_MARIO_RACCOON_SPIN_TAIL_RIGHT;
				else aniId = ID_ANI_MARIO_RACCOON_SPIN_TAIL_LEFT;
			}
			else if (vx == 0)
			{
				if (isHoldingShell)
				{
					if (nx > 0) aniId = ID_ANI_MARIO_RACCOON_IDLE_HOLD_SHELL_RIGHT;
					else aniId = ID_ANI_MARIO_RACCOON_IDLE_HOLD_SHELL_LEFT;
				}
				else
				{
					if (nx > 0) aniId = ID_ANI_MARIO_RACCOON_IDLE_RIGHT;
					else aniId = ID_ANI_MARIO_RACCOON_IDLE_LEFT;
				}
			}
			else if (vx > 0)
			{
				if (isHoldingShell)
				{
					if (ax < 0)
						aniId = ID_ANI_MARIO_RACCOON_WALK_HOLD_SHELL_LEFT;
					else
						aniId = ID_ANI_MARIO_RACCOON_WALK_HOLD_SHELL_RIGHT;
				}
				else if (ax < 0)
					aniId = ID_ANI_MARIO_RACCOON_STOP_LEFT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RACCOON_RUNNING_RIGHT;
				else/* if (ax == MARIO_ACCEL_WALK_X)*/
					aniId = ID_ANI_MARIO_RACCOON_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (isHoldingShell)
				{
					if (ax > 0)
						aniId = ID_ANI_MARIO_RACCOON_WALK_HOLD_SHELL_RIGHT;
					else
						aniId = ID_ANI_MARIO_RACCOON_WALK_HOLD_SHELL_LEFT;
				}
				else if (ax > 0)
					aniId = ID_ANI_MARIO_RACCOON_STOP_RIGHT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RACCOON_RUNNING_LEFT;
				else/* if (ax == -MARIO_ACCEL_WALK_X)*/
					aniId = ID_ANI_MARIO_RACCOON_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_RACCOON_IDLE_RIGHT;

	return aniId;
}

int CMario::GetAniIdFire()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(vx) == MARIO_MAX_RUNNING_SPEED)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_FIRE_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_FIRE_JUMP_RUN_LEFT;
		}
		else
		{
			if (isSitting)
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_FIRE_SIT_RIGHT;
				else
					aniId = ID_ANI_MARIO_FIRE_SIT_LEFT;
			}
			else if (vy > 0)
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_FIRE_FALLING_RIGHT;
				else
					aniId = ID_ANI_MARIO_FIRE_FALLING_LEFT;
			}
			else // vy < 0
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_FIRE_JUMP_WALK_RIGHT;
				else
					aniId = ID_ANI_MARIO_FIRE_JUMP_WALK_LEFT;
			}
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_FIRE_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_FIRE_SIT_LEFT;
		}
		else
			if (!(kickShell->IsTimeUp() || kickShell->IsStopped()))
			{
				if (nx > 0) aniId = ID_ANI_MARIO_FIRE_KICK_RIGHT;
				else aniId = ID_ANI_MARIO_FIRE_KICK_LEFT;
			}
			else if (vx == 0)
			{
				if (isHoldingShell)
				{
					if (nx > 0) aniId = ID_ANI_MARIO_FIRE_IDLE_HOLD_SHELL_RIGHT;
					else aniId = ID_ANI_MARIO_FIRE_IDLE_HOLD_SHELL_LEFT;
				}
				else
				{
					if (nx > 0) aniId = ID_ANI_MARIO_FIRE_IDLE_RIGHT;
					else aniId = ID_ANI_MARIO_FIRE_IDLE_LEFT;
				}
			}
			else if (vx > 0)
			{
				if (isHoldingShell)
				{
					if (ax < 0)
						aniId = ID_ANI_MARIO_WALK_HOLD_SHELL_LEFT;
					else
						aniId = ID_ANI_MARIO_WALK_HOLD_SHELL_RIGHT;
				}
				else if (ax < 0)
					aniId = ID_ANI_MARIO_FIRE_STOP_LEFT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_FIRE_RUNNING_RIGHT;
				else/* if (ax == MARIO_ACCEL_WALK_X)*/
					aniId = ID_ANI_MARIO_FIRE_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (isHoldingShell)
				{
					if (ax > 0)
						aniId = ID_ANI_MARIO_WALK_HOLD_SHELL_RIGHT;
					else
						aniId = ID_ANI_MARIO_WALK_HOLD_SHELL_LEFT;
				}
				else  if (ax > 0)
					aniId = ID_ANI_MARIO_FIRE_STOP_RIGHT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_FIRE_RUNNING_LEFT;
				else/* if (ax == -MARIO_ACCEL_WALK_X)*/
					aniId = ID_ANI_MARIO_FIRE_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_FIRE_IDLE_RIGHT;

	return aniId;
}

//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(vx) == MARIO_MAX_RUNNING_SPEED)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (isSitting)
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_SIT_RIGHT;
				else
					aniId = ID_ANI_MARIO_SIT_LEFT;
			}
			else if (isHoldingShell)
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_JUMP_HOLD_SHELL_RIGHT;
				else
					aniId = ID_ANI_MARIO_JUMP_HOLD_SHELL_LEFT;
			}
			else if (vy > 0)
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_FALLING_RIGHT;
				else
					aniId = ID_ANI_MARIO_FALLING_LEFT;
			}
			else // vy < 0
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
				else
					aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
			}
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (!(kickShell->IsTimeUp() || kickShell->IsStopped()))
			{
				if (nx > 0) aniId = ID_ANI_MARIO_KICK_RIGHT;
				else aniId = ID_ANI_MARIO_KICK_LEFT;
			}
			else if (vx == 0)
			{
				if (isHoldingShell)
				{
					if (nx > 0) aniId = ID_ANI_MARIO_IDLE_HOLD_SHELL_RIGHT;
					else aniId = ID_ANI_MARIO_IDLE_HOLD_SHELL_LEFT;
				}
				else
				{
					if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
					else aniId = ID_ANI_MARIO_IDLE_LEFT;
				}
			}
			else if (vx > 0)
			{
				if (isHoldingShell)
				{
					if (ax < 0)
						aniId = ID_ANI_MARIO_WALK_HOLD_SHELL_LEFT;
					else
						aniId = ID_ANI_MARIO_WALK_HOLD_SHELL_RIGHT;
				}
				else if (ax < 0)
					aniId = ID_ANI_MARIO_STOP_LEFT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else/* if (ax == MARIO_ACCEL_WALK_X)*/
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (isHoldingShell)
				{
					if (ax > 0)
						aniId = ID_ANI_MARIO_WALK_HOLD_SHELL_RIGHT;
					else
						aniId = ID_ANI_MARIO_WALK_HOLD_SHELL_LEFT;
				}
				else if (ax > 0)
					aniId = ID_ANI_MARIO_STOP_RIGHT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else/* if (ax == -MARIO_ACCEL_WALK_X)*/
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
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
	else if (level == MARIO_LEVEL_RACCOON)
		aniId = GetAniIdRaccoon();
	else if (level == MARIO_LEVEL_FIRE)
		aniId = GetAniIdFire();

	animations->Get(aniId)->Render(x, y);

	tail->Render();

	RenderBoundingBox();
	
	DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return; 

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) isSitting = false;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) isSitting = false;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		/*if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
		}*/
		if (isOnPlatform && level != MARIO_LEVEL_SMALL && state != MARIO_STATE_WALKING_RIGHT)
		{
			isSitting = true;
			DecelerateSlightly();
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE; // need?
		}
		break;

	case MARIO_STATE_IDLE:
		DecelerateSlightly();
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;

	case MARIO_STATE_ATTACK:
		if (spinTail->IsStopped())
		{
			spinTail->Start();
			tail->Attack();
		}
		break;
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_FIRE)
	{
		if (nx > 0)
			left = x ;
		else
			left = x - MARIO_BIG_IDLE_OFFSET_LEFT_L;

		if (isSitting)
		{
			top = y - MARIO_BIG_SITTING_OFFSET_TOP;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else 
		{
			top = y - MARIO_BIG_IDLE_OFFSET_TOP;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else if (level == MARIO_LEVEL_RACCOON)
	{
		if (nx > 0)
			left = x ;
		else
			left = x - MARIO_RACCOON_IDLE_OFFSET_LEFT_L;

		if (isSitting)
		{
			top = y - MARIO_BIG_SITTING_OFFSET_TOP;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else
		{
			top = y - MARIO_RACCOON_IDLE_OFFSET_TOP;
			right = left + MARIO_RACCOON_BBOX_WIDTH;
			bottom = top + MARIO_RACCOON_BBOX_HEIGHT;
		}
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		if (nx > 0)
			left = x - MARIO_SMALL_IDLE_OFFSET_LEFT;
		else
			left = x - MARIO_SMALL_IDLE_OFFSET_LEFT;
		top = y - MARIO_SMALL_IDLE_OFFSET_TOP;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

