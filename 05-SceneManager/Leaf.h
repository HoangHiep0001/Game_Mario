#pragma once
#include "GameObject.h"

#define LEAF_HIGHEST_OFFSET 32
#define LEAF_RIGHT_LIMIT_OFFSET 32
#define LEAF_DEFECT_SPEED_Y 1.4f
#define LEAF_SPEED_Y 0.45f
#define LEAF_FACTOR 0.25
#define LEAF_POW 35

#define LEAF_BBOX_WIDTH 16
#define LEAF_BBOX_HEIGHT 14

#define ID_ANI_LEAF_FALLING_LEFT 700
#define ID_ANI_LEAF_FALLING_RIGHT 800
class CLeaf : public CGameObject
{
	float highest;
	float leftLeaf, rightLeaf;

public:
	CLeaf(float x, float y, Type type) : CGameObject(x, y, type)
	{
		highest = y - LEAF_HIGHEST_OFFSET;
		leftLeaf = x;
		rightLeaf = x + LEAF_RIGHT_LIMIT_OFFSET;
		vy = -LEAF_DEFECT_SPEED_Y;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

