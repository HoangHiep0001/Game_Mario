#pragma once

#include "GameObject.h"

#define CELL_WIDTH  16
#define CELL_HEIGHT 16

class CGround : public CGameObject {
public:
	int width, height;

	CGround(float x, float y, Type type, int row_cell_num, int column_cell_num) : CGameObject(x, y, type) {
		width = row_cell_num * CELL_WIDTH;
		height = column_cell_num * CELL_HEIGHT;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

