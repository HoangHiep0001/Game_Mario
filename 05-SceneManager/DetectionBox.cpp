#include "DetectionBox.h"
#include "debug.h"

void CDetectionBox::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	float x, y;
	koopa->GetPosition(x, y);

	if (koopa->GetNx() > 0)
		left = x + DETECTION_BOX_OFFSET_LEFT_R;
	else
		left = x - DETECTION_BOX_OFFSET_LEFT_L;

	top = y - DETECTION_BOX_OFFSET_TOP;
	right = left + DETECTION_BOX_BBOX_WIDTH;
	bottom = top + DETECTION_BOX_BBOX_HEIGHT;
}

void CDetectionBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float ml, mt, mr, mb, bl, bt, br, bb; // main object (m) and blocking objects (b)
	GetBoundingBox(ml, mt, mr, mb);

	bool noCollision = true;
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT e = coObjects->at(i);
		if (dynamic_cast<CKoopa*>(e)) continue;

		e->GetBoundingBox(bl, bt, br, bb);
		if (CGameObject::CheckAABB(ml, mt, mr, mb, bl, bt, br, bb))
		{
			noCollision = false;
			break;
		}
	}

	if (noCollision && koopa->GetState() == KOOPA_STATE_WALKING)
		koopa->ChangeDirection();
}

void CDetectionBox::Render()
{
	//RenderBoundingBox();
}
