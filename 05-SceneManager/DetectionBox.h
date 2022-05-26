#pragma once
#include "GameObject.h"
#include "Koopa.h"

#define DETECTION_BOX_OFFSET_LEFT_R 2
#define DETECTION_BOX_OFFSET_LEFT_L 4
#define DETECTION_BOX_OFFSET_TOP 8

#define DETECTION_BOX_BBOX_WIDTH 2
#define DETECTION_BOX_BBOX_HEIGHT 28

class CDetectionBox : public CGameObject
{
public:
	CKoopa* koopa;

	CDetectionBox(CKoopa* koopa) { this->koopa = koopa; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
};

