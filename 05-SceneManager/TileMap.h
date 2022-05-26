#pragma once
#include "Sprites.h"
#include "Textures.h"
#include "Utils.h"
#include "Game.h"
#include "debug.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define TILE_CELL_WIDTH 16
#define TILE_CELL_HEIGHT 16

#define FIRST_TILE_CELL_ID 1

#define GAME_SCREEN_WIDTH	271
#define GAME_SCREEN_HEIGHT	264

class TileMap
{
	CSprites* sprites = CSprites::GetInstance();
	LPCWSTR map_file_path;
	LPCWSTR tileset_file_path;

	int id;
	int num_of_map_rows, num_of_map_cols;
	int num_of_tileset_rows, num_of_tileset_cols;

	int tileMap[100][200];

public:
	TileMap(int ID, LPCWSTR tileset_file_path, LPCWSTR map_file_path, int num_of_tileset_rows, int num_of_tileset_cols, int num_of_map_rows, int num_of_map_cols);
	int GetMapWidth();
	void LoadMapResource();
	void Load();
	void Render();
	~TileMap();
};

