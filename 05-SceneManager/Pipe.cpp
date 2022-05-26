#include "Pipe.h"

void CPipe::Render()
{
	int aniId = -1;
	if (pipeType == PIPE_TYPE_32)
		aniId = ID_ANI_PIPE_32;
	else if (pipeType == PIPE_TYPE_48)
		aniId = ID_ANI_PIPE_48;
	else if (pipeType == PIPE_TYPE_64)
		aniId = ID_ANI_PIPE_64;
	else if (pipeType == PIPE_TYPE_208_STRIPE)
		aniId = ID_ANI_PIPE_208_STRIPE;
	else if (pipeType == PIPE_TYPE_32_HIDDEN)
		aniId = ID_ANI_PIPE_32_HIDDEN;
	else if (pipeType == PIPE_TYPE_48_STRIPE)
		aniId = ID_ANI_PIPE_48_STRIPE;

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);

	//RenderBoundingBox();
}

void CPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - PIPE_BBOX_WIDTH / 2;
	r = l + PIPE_BBOX_WIDTH;

	if (pipeType == PIPE_TYPE_32 || pipeType == PIPE_TYPE_32_HIDDEN)
	{
		t = y - PIPE_BBOX_HEIGHT_32 / 2;
		b = t + PIPE_BBOX_HEIGHT_32;
	}
	else if (pipeType == PIPE_TYPE_48 || pipeType == PIPE_TYPE_48_STRIPE)
	{
		t = y - PIPE_BBOX_HEIGHT_48 / 2;
		b = t + PIPE_BBOX_HEIGHT_48;
	}
	else
	{
		t = y - PIPE_BBOX_HEIGHT_208 / 2;
		b = t + PIPE_BBOX_HEIGHT_208;
	}
}
