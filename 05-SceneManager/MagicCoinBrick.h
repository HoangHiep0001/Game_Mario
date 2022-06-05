#pragma once
#include "GameObject.h"
#include "Timer.h"

#define ANI_ID_BRONZE_BRICK	612

#define ANI_ID_IDLE_COIN	404
#define ANI_ID_ROTATE_COIN	405

#define TRANSFORMATION_TIME 7000

#define MAGIC_COIN_BRICK_STATE_NORMAL		222
#define MAGIC_COIN_BRICK_STATE_TRANSFORM	223

#define BRICK_BBOX_WIDTH	16
#define BRICK_BBOX_HEIGHT	16

#define COIN_BBOX_WIDTH		14
#define COIN_BBOX_HEIGHT	16

#define MAGIC_BRICK_STATE_BREAK 99

class CMagicCoinBrick : public CGameObject
{
	int blocking;
	Type initialType;
	BOOLEAN isBroken;
	BOOLEAN onTransformation;
	CTimer* transformationTime = new CTimer(TRANSFORMATION_TIME);

	vector<LPGAMEOBJECT> pieces;

public:
	CMagicCoinBrick(float x, float y, Type type);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	int IsBlocking() { return blocking; }
};


