#include "Ground.h"
#include "Textures.h"

void CGround::Render()
{
	//RenderBoundingBox();
}

void CGround::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + width;
	b = t + height;

	if (type == Type::COLOR_BOX)
		b = t + 4;
}
