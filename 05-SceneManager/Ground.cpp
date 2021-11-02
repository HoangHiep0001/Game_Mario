#include "Ground.h"
#include "debug.h"

Ground::Ground(float l, float t, float r, float b, int state)
{
	State = state;
	x = l;
	y = t;
	width = r;
	height = b;
	DebugOut(L"[INFO] Player object has been created!\n",x,y);
}

void Ground::Render()
{
	RenderBoundingBox();
}

void Ground::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
