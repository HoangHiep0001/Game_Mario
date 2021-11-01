#include "Map.h"
#include "Sprites.h"
#include "Utils.h"
#include "Game.h"

using namespace std;
#define TILE_SIZE 16.0f

Map::Map(int tileSetID, int rowMap, int columnMap, int rowTileSet, int columnTileSet, int totalTile)
{
	tileSet = CTextures::GetInstance()->Get(tileSetID);
	this->rowMap = rowMap;
	this->columnMap = columnMap;
	this->rowTileSet = rowTileSet;
	this->columnTileSet = columnTileSet;
	this->totalTile = totalTile;
	screenWidth = CGame::GetInstance()->GetBackBufferWidth();
	screenHeight = CGame::GetInstance()->GetBackBufferHeight();
}

Map::~Map()
{
}

void Map::SetCamera(float x, float y)
{
	this->camera_x = x;
	this->camera_y = y;
}

void Map::Render()
{
	int startingColumn = floor(camera_x / TILE_SIZE);
	int maxColumn = ceil((screenWidth + camera_x) / TILE_SIZE);
	int startingRow = floor(camera_y / TILE_SIZE);
	int maxRow = ceil((screenHeight + camera_y) / TILE_SIZE);
	//DebugOut(L" start = %f, max = %f\n", startingColumn, maxColumn);

	if (maxColumn >= columnMap) maxColumn = columnMap;
	if (maxRow >= rowMap) maxRow = rowMap;
	for (int currentRow = startingRow; currentRow < maxRow; currentRow++)
		for (int currentColumn = startingColumn; currentColumn < maxColumn; currentColumn++)
			tiles.at(tileMap[currentRow][currentColumn] - 1)->Draw(currentColumn * TILE_SIZE, currentRow * TILE_SIZE);
}

void Map::SetTileMapData(std::vector<std::vector<int>> tileMapData)
{
	tileMap = tileMapData;
}


void Map::ExtractTileFromTileSet()
{
	for (int tileNum = 0; tileNum < totalTile; tileNum++)
	{
		int left = tileNum % columnTileSet * TILE_SIZE;
		int top = tileNum / columnTileSet * TILE_SIZE;
		int right = left + TILE_SIZE;
		int bottom = top + TILE_SIZE;
		LPSPRITE newTile = new CSprite(tileNum, left, top, right, bottom, tileSet);
		this->tiles.push_back(newTile);
	}
}

float Map::GetMapWidth()
{
	return columnMap * TILE_SIZE;
}

float Map::GetMapHeight()
{
	return rowMap * TILE_SIZE;
}
