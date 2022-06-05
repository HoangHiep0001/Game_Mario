#include "MagicCoinBrick.h"
#include "BrokenPiece.h"
#include "debug.h"

CMagicCoinBrick::CMagicCoinBrick(float x, float y, Type type) : CGameObject(x, y, type)
{
	isBroken = false;
	onTransformation = false;
	initialType = type;

	if (type == Type::COIN) blocking = 0;
	else blocking = 1;
}

void CMagicCoinBrick::Render()
{
	int aniId = -1;

	if (type == Type::COIN)
	{
		if (initialType == Type::COIN) aniId = ANI_ID_ROTATE_COIN;
		else aniId = ANI_ID_IDLE_COIN;
	}
	else
		aniId = ANI_ID_BRONZE_BRICK;

	if (!isBroken)
		CAnimations::GetInstance()->Get(aniId)->Render(x, y);

	for (LPGAMEOBJECT piece : pieces)
		piece->Render();

	//RenderBoundingBox();
}

void CMagicCoinBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	for (int i = 0; i < pieces.size(); i++)
	{
		pieces[i]->Update(dt, coObjects);

		if (pieces[i]->IsDeleted())
			pieces.erase(pieces.begin() + i);
	}
}

void CMagicCoinBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (isBroken) return;

	if (type == Type::COIN)
	{
		l = x - COIN_BBOX_WIDTH / 2;
		t = y - COIN_BBOX_HEIGHT / 2;
		r = l + COIN_BBOX_WIDTH;
		b = t + COIN_BBOX_HEIGHT;
	}
	else
	{
		l = x - BRICK_BBOX_WIDTH / 2;
		t = y - BRICK_BBOX_HEIGHT / 2;
		r = l + BRICK_BBOX_WIDTH;
		b = t + BRICK_BBOX_HEIGHT;
	}
}

void CMagicCoinBrick::SetState(int state)
{
	if (state == MAGIC_BRICK_STATE_BREAK)
	{
		CBrokenPiece* topLeftPiece = new CBrokenPiece(x - 5, y - 6, -1, 2);
		CBrokenPiece* topRightPiece = new CBrokenPiece(x + 5, y - 6, 1, 2);
		CBrokenPiece* bottomLeftPiece = new CBrokenPiece(x - 5, y + 4, -1);
		CBrokenPiece* bottomRightPiece = new CBrokenPiece(x + 5, y + 4, 1);

		pieces = { topLeftPiece, topRightPiece, bottomLeftPiece, bottomRightPiece };
		isBroken = true;
	}

	CGameObject::SetState(state);
}