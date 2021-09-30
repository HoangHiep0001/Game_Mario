#include "Map.h"
#include "Sprites.h"

#define SCREEN_HEIGHT 200
#define TILE_SIZE 16
Map::Map(int ID, LPCWSTR File_data, int rowMap, int  columnMap, int rowTileSet, int columnTileSet)
{
	this->ID = ID;
	this->MapData= File_data;
	this->rowMap = rowMap;
	this->columnMap = columnMap;
	this->rowTileSet = rowTileSet;
	this->columnTileSet = columnTileSet;
	
	LoadSpriteMap();
	LoadMap();
}
void Map::LoadMap()
{
	ifstream f;
	f.open(MapData);
	for (int i = 0; i < rowMap; i++)
	{
		for (int j = 0; j < columnMap; j++)
		{
			f >> tileMap[i][j];
		}
	}
	f.close();
}

void Map::LoadSpriteMap()
{
	CTextures* texture = CTextures::GetInstance();
	LPTEXTURE texMap = texture->Get(ID);
	int id_sprite = 1;
	for (int i = 0; i < rowTileSet; i++)
	{
		for (int j = 0; j < columnTileSet; j++)
		{
			CSprites::GetInstance()->Add(id_sprite, TILE_SIZE * j, TILE_SIZE * i, TILE_SIZE * (j+1), TILE_SIZE * (i+1), texMap);
			id_sprite++;
		}
	}
}

void Map::Render()
{
	for (int i = 0; i < rowMap; i++)
	{
		for (int j = 0; j < columnMap; j++)
		{
			CSprites::GetInstance()->Get(tileMap[i][j])->Draw(int(TILE_SIZE * j), int(TILE_SIZE * i) - SCREEN_HEIGHT);
		}
	}
}
