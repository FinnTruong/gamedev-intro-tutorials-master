#pragma once

#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Scence.h"
#include "Tilemap.h"
#include "HUD.h"

#include "WorldMapMario.h"
#include "Cactus.h"
#include "HammerBrother.h"
#include "SpeechBubble.h"


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_TILEMAP	7
#define SCENE_SECTION_GRID		8

#define OBJECT_TYPE_MARIO					0
#define OBJECT_TYPE_CACTUS					1
#define OBJECT_TYPE_HAMMER_BROTHER			2
#define OBJECT_TYPE_SPEECH_BUBBLE			3


#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024

#define WORLD_MAP_TILEMAP_X_OFFSET 0
#define WORLD_MAP_TILEMAP_Y_OFFSET -20

#define GRID_TOTAL_ROWS			9
#define GRID_TOTAL_COLUMNS		11


class WorldSelectionScene : public CScene
{
protected:
	Tilemap* tilemap;
	HUD* hud;
	WorldMapMario* player;			// A play scene has to have player, right? 

	vector<vector<int>> mapGrid;

	int grid_rows = 0, grid_columns = 0;

	int curNodeX = 0;
	int curNodeY = 0;

	bool hasLoadGrid = false;

	vector<LPGAMEOBJECT> objects;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TILEMAP(string line);
	void _ParseSection_GRID(string line);

public:
	WorldSelectionScene(int id, LPCWSTR filePath);

	void Load();
	void Update(DWORD dt);
	void Render();
	void Unload();

	void KeyState(BYTE* states) {}
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);

	void LoadGrid(wstring filePath);
	bool CheckIfCanMove(Vector2);
};


enum MapNode
{	
	EMPTY,
	BLOCK,
	STOP,
	INTERACTABLE,
	START
};
