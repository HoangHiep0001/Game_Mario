#include <d3dx9.h>
#include <algorithm>


#include "debug.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;	
	state = -1;
	isDeleted = false;
}
void CGameObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	dx = vx * dt;
	dy = vy * dt;
}
void CGameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l,t,r,b; 

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy; 
	CGame::GetInstance()->GetCamPos(cx, cy);

	CGame::GetInstance()->Draw(x - cx, y - cy, bbox, &rect, BBOX_ALPHA);
}


bool CGameObject::AABB(float l, float t, float r, float b, float l1, float t1, float r1, float b1)
{
	// kiểm tra 2 hình chữ nhật có đang chèn nhau hay không
	float left = l1 - r;
	float top = b1 - t;
	float right = r1 - l;
	float bottom = t1 - b;
	//  xét ngược lại cho nhanh hơn
	return !(left > 0 || right < 0 || top < 0 || bottom > 0);
}
CGameObject::~CGameObject()
{

}