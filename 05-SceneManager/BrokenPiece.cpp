#include "BrokenPiece.h"
#include "debug.h"

CBrokenPiece::CBrokenPiece(float x, float y, int nx, int deflectFactorY)
{
	this->x = x;
	this->y = y;
	vx = BROKEN_PIECE_SPEED_X * nx;
	vy = -BROKEN_PIECE_DEFLECT_SPEED_Y * deflectFactorY;
}

void CBrokenPiece::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	DebugOut(L"PIECE UPDATE\n");
	vy += BROKEN_PIECE_GRAVITY * dt;

	x += vx * dt;
	y += vy * dt;

	if (y > CGame::GetInstance()->GetCamPosY() + GAME_SCREEN_HEIGHT)
		Delete();
}

void CBrokenPiece::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_BROKEN_PIECE)->Render(x, y);
}