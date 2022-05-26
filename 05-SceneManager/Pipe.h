#pragma once
#include "GameObject.h"

#define PIPE_BBOX_WIDTH	32
#define PIPE_BBOX_HEIGHT_32	32
#define PIPE_BBOX_HEIGHT_48	48
#define PIPE_BBOX_HEIGHT_208 208

#define	PIPE_TYPE_32 1
#define	PIPE_TYPE_48 2
#define	PIPE_TYPE_64 3
#define	PIPE_TYPE_208_STRIPE 4
#define	PIPE_TYPE_32_HIDDEN 5
#define	PIPE_TYPE_48_STRIPE 6

#define ID_ANI_PIPE_32 400
#define ID_ANI_PIPE_48 401
#define ID_ANI_PIPE_64 402
#define ID_ANI_PIPE_208_STRIPE 403
#define ID_ANI_PIPE_32_HIDDEN 399
#define ID_ANI_PIPE_48_STRIPE 398

class CPipe : public CGameObject
{
	int pipeType;

public:
	CPipe(float x, float y, Type type, int pipeType) : CGameObject(x, y, type)
	{
		this->pipeType = pipeType;
	}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

