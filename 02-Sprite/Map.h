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

	LPCWSTR MapData;
	int tileMap[200][200];
	
public:
	Map(int ID, LPCWSTR File_data, int rowMap, int  columnMap, int rowTileSet, int columnTileSet );
	~Map() {};
	void LoadMap();
	void LoadSpriteMap();
	void Render();
};

