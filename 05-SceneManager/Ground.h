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
public:
	Ground(float l, float t, float r, float b, int state);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetGroundState() { return this->State; }
};

