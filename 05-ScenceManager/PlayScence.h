#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "QuestionBlock.h"
#include "Ground.h"
#include "Player.h"
#include "Goomba.h"
#include "Paragoomba.h"
#include "KoopaTroopa.h"
#include "KoopaParaTroopa.h"
#include "PiranhaPlant.h"
#include "VenusFireTrap.h"
#include "Tilemap.h"
#include "Item.h"
#include "Mushroom.h"
#include "Leaf.h"
#include "Flower.h"
#include "Coin.h"
#include "HUD.h"
#include "Pipe.h"
#include "Goal.h"
#include "Grid.h"


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_TILEMAP	7

#define OBJECT_TYPE_MARIO					0
#define OBJECT_TYPE_GROUND					1
#define OBJECT_TYPE_QUESTION_BLOCK			2
#define OBJECT_TYPE_ONEWAYPLATFORM			3
#define OBJECT_TYPE_GOOMBA					4
#define OBJECT_TYPE_KOOPAS					5
#define OBJECT_TYPE_FIREBALL				6
#define OBJECT_TYPE_LEAF					7
#define OBJECT_TYPE_COIN					8
#define OBJECT_TYPE_MUSHROOM				9
#define OBJECT_TYPE_FLOWER					10
#define OBJECT_TYPE_PIRANHA_PLANT			11
#define OBJECT_TYPE_VENUS_FIRE_TRAP			12
#define OBJECT_TYPE_PARAGOOMBA				13
#define OBJECT_TYPE_PIPE					14
#define OBJECT_TYPE_BRICK					15
#define OBJECT_TYPE_P_BLOCK					16
#define OBJECT_TYPE_PARATROOPA				17
#define OBJECT_TYPE_GOAL					18

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024

#define PLAY_TILEMAP_X_OFFSET 0
#define PLAY_TILEMAP_Y_OFFSET -162

#define SORTING_LAYERS_SIZE	2

const int CELL_SIZE = 64;

class CPlayScene: public CScene
{
protected: 
	Tilemap* tilemap = nullptr;
	HUD* hud = nullptr;
	Player *player = nullptr;				// A play scene has to have player, right? 
	Grid* grid = nullptr; // Grid for spatial partitioning for collision

	ULONGLONG startSceneTime = 0;

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> activeObjects;
	vector<LPGAMEOBJECT> disabledObjects;

	bool isUnloading = false;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TILEMAP(string line);

	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	void Load();
	void Update(DWORD dt);
	void Render();
	void Unload();


	void KeyState(BYTE *states);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);


	//Add new game obj to active list
	void AddNewGameObject(LPGAMEOBJECT obj)
	{
		objects.push_back(obj);
	}

	//Remove game obj from active list
	void DisableGameObject(LPGAMEOBJECT gameObj)
	{
		gameObj->SetActive(false);
		for (size_t i = 0; i < objects.size(); i++)
		{
			disabledObjects.push_back(objects[i]);
		}

		for (size_t i = 0; i < objects.size(); i++)
		{
			if (objects[i] == gameObj)
			{
				objects.erase(objects.begin() + i);
			}
		}
	}

	int GetTimer();
	Tilemap* GetTilemap() { return tilemap; }
	Player * GetPlayer() { return player; } 

	vector<LPGAMEOBJECT> GetObjList()
	{
		return objects;
	}

};

//class CPlayScenceKeyHandler : public CScenceKeyHandler
//{
//public: 
//	virtual void KeyState(BYTE *states);
//	virtual void OnKeyDown(int KeyCode);
//	virtual void OnKeyUp(int KeyCode);
//	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
//};