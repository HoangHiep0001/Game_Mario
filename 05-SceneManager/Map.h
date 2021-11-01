#pragma once
#include <d3d10.h>
#include"Textures.h"
#include"Sprites.h"
#include"Textures.h"
#include"Utils.h"
#include <iostream>
#include <fstream>
#include<string>
#include <sstream>
#include"Game.h"
class Map
{
	float camera_x;
	float camera_y;
	int rowMap, columnMap;
	int rowTileSet, columnTileSet;

	int totalTile;

	LPTEXTURE tileSet;
	vector<LPSPRITE> tiles;
	std::vector<std::vector<int>> tileMap;
	int screenWidth;
	int screenHeight;
public:
	Map(int tileSetID, int rowMap, int columnMap, int rowTileSet, int  columnTileSet, int totalTile);
	~Map();
	void Render();
	void SetTileMapData(std::vector<std::vector<int>> tileMapData);
	void SetCamera(float x, float y);
	void ExtractTileFromTileSet();
	float GetMapWidth();
	float GetMapHeight();
};

