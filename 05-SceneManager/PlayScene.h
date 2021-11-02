#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Map.h"
//#include "Koopas.h"


class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;					
	Map* tileMap;
	RECT camera;
	RECT mapCamera;
	unordered_map<int, RECT> mapCameras;
	unordered_map<int, RECT> Cameras;
	unordered_map<int, int> isSpecialCamera;
	vector<LPGAMEOBJECT> objects;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TILEMAP_DATA(string line);
	void LoadAssets(LPCWSTR assetFile);
	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }
	RECT GetCamera() { return this->camera; }
	void SetMapCamera(RECT map) { this->mapCamera = map; }
	void SetCamera(RECT cam) { this->camera = cam; }
	unordered_map<int, RECT> GetListMapCamera() {
		return this->mapCameras;
	}
	unordered_map<int, int> GetSpecialCamera() { return this->isSpecialCamera; }
	unordered_map<int, RECT> GetListCamera() {
		return this->Cameras;
	}
	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CPlayScene* LPPLAYSCENE;

