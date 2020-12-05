#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "QuestionMarkBrick.h"
#include "Ground.h"
#include "Player.h"
#include "Goomba.h"
#include "Paragoomba.h"
#include "KoopaTroopa.h"
#include "PiranhaPlant.h"
#include "VenusFireTrap.h"
#include "Tilemap.h"
#include "Item.h"
#include "Mushroom.h"
#include "Leaf.h"
#include "Flower.h"
#include "Coin.h"


class CPlayScene: public CScene
{
protected: 
	Tilemap* tilemap;
	Player *player;					// A play scene has to have player, right? 

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> items;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TILEMAP(string line);

	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);


	//Add new game obj to active list
	void AddNewGameObject(LPGAMEOBJECT obj)
	{
		objects.push_back(obj);
	}

	//Remove game obj from active list
	void DisableGameObject(LPGAMEOBJECT gameObj)
	{
		for (size_t i = 0; i < objects.size(); i++)
		{
			if (objects[i] == gameObj)
			{
				objects.erase(objects.begin() + i);
			}
		}
	}

	Player * GetPlayer() { return player; } 

	friend class CPlayScenceKeyHandler;
};

//class CPlayScenceKeyHandler : public CScenceKeyHandler
//{
//public: 
//	virtual void KeyState(BYTE *states);
//	virtual void OnKeyDown(int KeyCode);
//	virtual void OnKeyUp(int KeyCode);
//	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
//};

