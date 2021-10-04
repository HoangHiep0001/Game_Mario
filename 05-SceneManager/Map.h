#pragma once
#include <iostream>
#include <fstream>
#include "Sprites.h"
#include "Textures.h"
#include "Game.h"
class Map
{

	int ID;
	int rowMap, columnMap;
	int rowTileSet, columnTileSet;
	int totalTile;
	float camera_x;
	float camera_y;
	LPCWSTR MapData;
	int tileMap[200][200];
	int screenWidth;
	int screenHeight;
public:
	Map(int ID, LPCWSTR File_data, int rowMap, int  columnMap, int rowTileSet, int columnTileSet);
	~Map() {};
	void LoadMap();
	void LoadSpriteMap();
	void Render();
	void SetCamera(float x, float y);
	float GetMapWidth();
	float GetMapHeight();
};

