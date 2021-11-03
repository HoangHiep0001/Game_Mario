#pragma once
#include "GameObject.h"

/*
	Object that triggers scene switching
*/
class Ground : public CGameObject
{
	int State;
	int width;
	int height;
	int length;
public:
	Ground(float x, float y,
		float cell_width, float cell_height, int length, int state) :CGameObject(x, y)
	{
		this->State = state;
		this->length = length;
		this->width = cell_width;
		this->height = cell_height;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
	int GetGroundState() { return this->State; }
};

