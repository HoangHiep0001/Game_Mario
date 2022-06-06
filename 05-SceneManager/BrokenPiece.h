#pragma once
#include "GameObject.h"

#define BROKEN_PIECE_SPEED_X			0.07f
#define BROKEN_PIECE_DEFLECT_SPEED_Y	0.16f
#define BROKEN_PIECE_GRAVITY			0.0009f

#define ID_ANI_BROKEN_PIECE 901

#define LOW 1
class CBrokenPiece : public CGameObject
{
public:
	CBrokenPiece(float x, float y, int nx, int deflectFactorY = LOW);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b) { l = t = r = b = 0; }
};


