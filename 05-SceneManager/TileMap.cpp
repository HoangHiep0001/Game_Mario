#include "TileMap.h"

TileMap::TileMap(int ID, LPCWSTR tileset_file_path, LPCWSTR map_file_path, int num_of_tileset_rows, int num_of_tileset_cols, int num_of_map_rows, int num_of_map_cols)
{
	this->id = ID;
	this->tileset_file_path = tileset_file_path;
	this->map_file_path = map_file_path;
	this->num_of_tileset_rows = num_of_tileset_rows;
	this->num_of_tileset_cols = num_of_tileset_cols;
	this->num_of_map_rows = num_of_map_rows;
	this->num_of_map_cols = num_of_map_cols;

	LoadMapResource();
	Load();
}

int TileMap::GetMapWidth()
{
	return num_of_map_cols * TILE_CELL_WIDTH;
}

void TileMap::LoadMapResource()
{
	CTextures* texture = CTextures::GetInstance();
	texture->Add(id, tileset_file_path);

	LPTEXTURE texMap = texture->Get(id);

	int tile_cell_id = FIRST_TILE_CELL_ID;
	for (UINT i = 0; i < num_of_tileset_rows; i++)
	{
		for (UINT j = 0; j < num_of_tileset_cols; j++)
		{
			int spriteId = id + tile_cell_id;
			sprites->Add(spriteId, TILE_CELL_WIDTH * j, TILE_CELL_HEIGHT * i, TILE_CELL_WIDTH * (j + 1), TILE_CELL_HEIGHT * (i + 1), texMap);
			tile_cell_id++;
		}
	}
}

void TileMap::Load()
{
	DebugOut(L"[INFO] Start loading map resources from : %s \n", map_file_path);

	ifstream fs(map_file_path, ios::in);

	if (fs.fail())
	{
		DebugOut(L"[ERROR] TileMap::Load_MapData failed: ID=%d", id);
		fs.close();
		return;
	}

	for (int i = 0; i < num_of_map_rows; i++)
	{
		for (int j = 0; j < num_of_map_cols; j++)
			fs >> tileMap[i][j];
	}

	fs.close();

	DebugOut(L"[INFO] Done loading map resources %s\n", map_file_path);
}

void TileMap::Render()
{
	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	int left = (int)cx / TILE_CELL_WIDTH;
	int right = left + (GAME_SCREEN_WIDTH / TILE_CELL_WIDTH);

	int top = (int)cy / TILE_CELL_HEIGHT;
	int bottom = top + (GAME_SCREEN_HEIGHT / TILE_CELL_HEIGHT);

	for (UINT i = top; i <= bottom; i++)
	{
		for (UINT j = left; j <= right; j++)
		{
			float x = TILE_CELL_WIDTH * j + TILE_CELL_WIDTH / 2;
			float y = TILE_CELL_HEIGHT * i + TILE_CELL_HEIGHT / 2;
			sprites->Get(tileMap[i][j] + id)->Draw(x, y);
		}
	}
}

TileMap::~TileMap()
{
}
