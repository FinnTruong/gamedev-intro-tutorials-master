#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "GameGlobal.h"
#include "Fireball.h"
#include "OneWayPlatform.h"
#include "Brick.h"
#include "PBlock.h"
#include "MovingPlatform.h"
#include "SledgeBro.h"



CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	sceneType = 2;
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/



#pragma region  PARSE GAME DATA

void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextureDatabase::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str()) + l;
	int b = atoi(tokens[4].c_str()) + t;
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextureDatabase::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSpriteDatabase::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (auto i = 1; (unsigned)i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (int i = 1; (unsigned)i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());
		
		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSetDatabase::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSetDatabase * animation_sets = CAnimationSetDatabase::GetInstance();

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new Player(x,y); 
		player = (Player*)obj;  

		DebugOut(L"[INFO] Player object created!\n");
		break;

	case OBJECT_TYPE_GOOMBA: obj = new Goomba(); break;

	case OBJECT_TYPE_PARAGOOMBA: obj = new Paragoomba(); break;

	case OBJECT_TYPE_QUESTION_BLOCK: 
	{
		int hasPowerup = (int)atof(tokens[4].c_str());
		int hasOneUp = (int)atof(tokens[5].c_str());
		obj = new QuestionBlock(x, y, hasPowerup, hasOneUp);
	}
	break;

	case OBJECT_TYPE_BRICK:	obj = new Brick(); break;

	case OBJECT_TYPE_P_BLOCK: obj = new PBlock(x,y); break;

	case OBJECT_TYPE_KOOPAS: 
	{
		int type = atoi(tokens[4].c_str());
		obj = new KoopaTroopa(x, y, type);
	}
	break;

	case OBJECT_TYPE_PARATROOPA: 
	{
		int type = atoi(tokens[4].c_str());
		obj = new KoopaParatroopa(x,y,type); break;
	}

	case OBJECT_TYPE_PIRANHA_PLANT: obj = new PiranhaPlant(x, y); break;

	case OBJECT_TYPE_VENUS_FIRE_TRAP: 
	{
		int type = (int)atof(tokens[4].c_str());
		obj = new VenusFireTrap(x, y, type); 
	}
	break;

	case OBJECT_TYPE_SLEDGE_BRO: obj = new SledgeBro(x,y); break;

	case OBJECT_TYPE_GROUND:
	{
		int width = (int)atof(tokens[4].c_str());
		int height= (int)atof(tokens[5].c_str());
		obj = new Ground(width,height);
;
	}
	break;

	case OBJECT_TYPE_ONEWAYPLATFORM:
	{
		int width = (int)atof(tokens[4].c_str());
		int height = (int)atof(tokens[5].c_str());
		obj = new OneWayPlatform(width, height);
	}
	break;

	case OBJECT_TYPE_PIPE:
	{
		int width = (int)atof(tokens[4].c_str());
		int height = (int)atof(tokens[5].c_str());
		int type = (int)atof(tokens[6].c_str());
		bool hasSecretEntrance = atof(tokens[7].c_str());
		obj = new Pipe(width, height, type, hasSecretEntrance);
	}
	break;
	case OBJECT_TYPE_LEAF: obj = new Leaf(x, y); break;
	case OBJECT_TYPE_COIN: obj = new Coin(x, y); break;
	case OBJECT_TYPE_MUSHROOM: obj = new Mushroom(x, y); break;
	case OBJECT_TYPE_GOAL: obj = new Goal(); break;
	case OBJECT_TYPE_MOVING_PLATFORM: obj = new MovingPlatform(); break;
	case OBJECT_TYPE_PORTAL:
		{	
			float r = (float)atof(tokens[4].c_str());
			float b = (float)atof(tokens[5].c_str());
			int scene_id = (int)atoi(tokens[6].c_str());
			obj = new CPortal(x, y, r, b, scene_id);
		}
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
	grid->InitObjects(obj);

}

void CPlayScene::_ParseSection_TILEMAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	wstring filePath_texture = ToWSTR(tokens[1]);
	wstring filePath_data = ToWSTR(tokens[2]);
	int num_row_on_texture = atoi(tokens[3].c_str());
	int num_col_on_texture = atoi(tokens[4].c_str());
	int num_row_on_tilemap = atoi(tokens[5].c_str());
	int num_col_on_tilemap = atoi(tokens[6].c_str());
	int tileset_width = atoi(tokens[7].c_str());
	int tileset_height = atoi(tokens[8].c_str());

	tilemap = new Tilemap(ID, filePath_texture.c_str(), filePath_data.c_str(), num_row_on_texture, num_col_on_texture, num_row_on_tilemap, num_col_on_tilemap, tileset_width, tileset_height);
	//grid = new Grid(tilemap->GetWidthTileMap(), tilemap->GetHeightTileMap());
}

void CPlayScene::_ParseSection_GRID(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return; // skip invalid lines

	wstring gridInfo = ToWSTR(tokens[0]);	

	ifstream f;
	f.open(gridInfo);

	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[GRID_SIZE]")
		{
			section = SCENE_SECTION_GRID;
			continue;
		}
		if (line == "[OBJECTS]")
		{
			section = SCENE_SECTION_OBJECTS;
			continue;
		}

		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_GRID: LoadGrid(line); break;
		case SCENE_SECTION_OBJECTS: LoadGridObjects(line); break;
		}
	}

	f.close();
}

void CPlayScene::LoadGrid(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	int grid_width = atoi(tokens[0].c_str());
	int grid_height = atoi(tokens[1].c_str());

	grid = new Grid(grid_width, grid_height);
}

void CPlayScene::LoadGridObjects(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int cell_x = atoi(tokens[0].c_str());
	int cell_y = atoi(tokens[1].c_str());
	int object_type = atoi(tokens[2].c_str());
	float x = (float)atof(tokens[3].c_str());
	float y = (float)atof(tokens[4].c_str());

	int ani_set_id = atoi(tokens[5].c_str());

	CAnimationSetDatabase* animation_sets = CAnimationSetDatabase::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new Player(x, y);
		player = (Player*)obj;

		DebugOut(L"[INFO] Player object created!\n");
		break;

	case OBJECT_TYPE_GOOMBA: obj = new Goomba(); break;

	case OBJECT_TYPE_PARAGOOMBA: obj = new Paragoomba(); break;

	case OBJECT_TYPE_QUESTION_BLOCK:
	{
		int hasPowerup = (int)atof(tokens[6].c_str());
		int hasOneUp = (int)atof(tokens[7].c_str());
		obj = new QuestionBlock(x, y, hasPowerup, hasOneUp);
	}
	break;

	case OBJECT_TYPE_BRICK:	obj = new Brick(); break;

	case OBJECT_TYPE_P_BLOCK: obj = new PBlock(x, y); break;

	case OBJECT_TYPE_KOOPAS:
	{
		int type = atoi(tokens[6].c_str());
		obj = new KoopaTroopa(x, y, type);
	}
	break;

	case OBJECT_TYPE_PARATROOPA:
	{
		int type = atoi(tokens[6].c_str());
		obj = new KoopaParatroopa(x,y,type); break;
	}

	case OBJECT_TYPE_PIRANHA_PLANT: obj = new PiranhaPlant(x, y); break;

	case OBJECT_TYPE_VENUS_FIRE_TRAP:
	{
		int type = (int)atof(tokens[6].c_str());
		obj = new VenusFireTrap(x, y, type);
	}
	break;
	case OBJECT_TYPE_SLEDGE_BRO: obj = new SledgeBro(x,y); break;
	case OBJECT_TYPE_GROUND:
	{
		int width = (int)atof(tokens[6].c_str());
		int height = (int)atof(tokens[7].c_str());
		obj = new Ground(width, height);
		;
	}
	break;

	case OBJECT_TYPE_ONEWAYPLATFORM:
	{
		int width = (int)atof(tokens[6].c_str());
		int height = (int)atof(tokens[7].c_str());
		obj = new OneWayPlatform(width, height);
	}
	break;
	case OBJECT_TYPE_MOVING_PLATFORM: obj = new MovingPlatform(); break;
	case OBJECT_TYPE_PIPE:
	{
		int width = (int)atof(tokens[6].c_str());
		int height = (int)atof(tokens[7].c_str());
		int type = (int)atof(tokens[8].c_str());
		bool hasSecretEntrance = atof(tokens[9].c_str());
		obj = new Pipe(width, height, type, hasSecretEntrance);
	}
	break;
	case OBJECT_TYPE_LEAF: obj = new Leaf(x, y); break;
	case OBJECT_TYPE_COIN: obj = new Coin(x, y); break;
	case OBJECT_TYPE_MUSHROOM: obj = new Mushroom(x, y); break;
	case OBJECT_TYPE_GOAL: obj = new Goal(); break;
	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[6].c_str());
		float b = (float)atof(tokens[7].c_str());
		int scene_id = (int)atoi(tokens[8].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
	grid->InitObjects(obj,cell_x, cell_y);
}
#pragma endregion

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	isUnloading = false;
	startSceneTime = GetTickCount64();

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]")
		{
			section = SCENE_SECTION_TEXTURES;
			continue; 
		}
		if (line == "[SPRITES]")
		{ 
			section = SCENE_SECTION_SPRITES; 
			continue;
		}
		if (line == "[ANIMATIONS]") 
		{ 
			section = SCENE_SECTION_ANIMATIONS; 
			continue;
		}
		if (line == "[ANIMATION_SETS]") 
		{ 
			section = SCENE_SECTION_ANIMATION_SETS;
			continue; 
		}
		if (line == "[OBJECTS]") 
		{ 
			section = SCENE_SECTION_OBJECTS;
			continue;
		}
		if (line == "[TILEMAP]")
		{
			section = SCENE_SECTION_TILEMAP;
			continue;
		}
		if (line == "[GRID]")
		{
			section = SCENE_SECTION_GRID;
			continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
			case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
			case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
			case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
			//case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
			case SCENE_SECTION_TILEMAP: _ParseSection_TILEMAP(line); break;
			case SCENE_SECTION_GRID: _ParseSection_GRID(line); break;
		}
	}

	f.close();

	CTextureDatabase::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	CTextureDatabase::GetInstance()->Add(ID_TEX_HUD_BG, L"textures\\UI\\squarebox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	int cam_x, cam_y;
	camera->GetPosition(cam_x, cam_y);

	if (player->y >= cam_y + SCREEN_HEIGHT && !player->hasHitGoal)
		player->isDead = true;

	grid->Update();
	vector<LPGAMEOBJECT> coObjects;
	coObjects.clear();


	activeObjects = GetActiveGameObjects();


	for (size_t i = 0; i < activeObjects.size(); i++)
	{		
		if (isUnloading)
			return;
		activeObjects[i]->Update(dt, &activeObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 


	// Update camera to follow mario
	camera->Update(dt);



}

void CPlayScene::Render()
{
	tilemap->Draw(PLAY_TILEMAP_X_OFFSET,PLAY_TILEMAP_Y_OFFSET);
	//Render objects

	
	activeObjects = GetActiveGameObjects();

	for (size_t j = 0; j < SORTING_LAYERS_SIZE; j++)
	{
		for (int i = 0; (unsigned)i < activeObjects.size(); i++)
		{
			if (activeObjects[i]->sortingLayer == j)
				activeObjects[i]->Render();

		}
	}

	DrawEndLevelText();
	hud->Draw();


}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	isUnloading = true;

	for (int i = 0; (unsigned)i < objects.size(); i++)
		delete objects[i];

	objects.clear();

	/*for (int i = 0; (unsigned)i < disabledObjects.size(); i++)
		delete disabledObjects[i];

	disabledObjects.clear();*/

	//Mario->listFireball.clear();
	player = NULL;
	grid = nullptr;
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScene::DrawEndLevelText()
{
	if (!Mario->hasHitGoal)
		return;

	if (Mario->hasHitGoal)
	{
		if (GetTickCount64() - Mario->hitGoalTime < 1000)
			return;
	}

	int cam_x, cam_y;
	CGame::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition(cam_x, cam_y);

	auto sprites = CSpriteDatabase::GetInstance();

	auto text = sprites->Get(10);
	text->Draw(1, (float)(cam_x + SCREEN_WIDTH / 2 - 65), (float)(cam_y + 36));

	auto card = sprites->Get(51);
	auto cardCollected = CGame::GetInstance()->GetCards();
	for(int i = 2;  i >= 0; i--)
	{
		if (cardCollected[i] != 0)
		{
			card = sprites->Get(51 + cardCollected[i]);
			break;
		}
		else
			card = sprites->Get(51);
	}

	card->Draw(1, (float)(cam_x + SCREEN_WIDTH / 2 - 65 + 116), (float)(cam_y + 51));
}

void CPlayScene::OnKeyDown(int key)
{
	keyCode[key] = true;
	player->OnKeyDown(key);
}

void CPlayScene::OnKeyUp(int key)
{
	keyCode[key] = false;
	player->OnKeyUp(key);
}

void CPlayScene::KeyState(BYTE *states)
{
	/*Player *mario = ((CPlayScene*)scence)->GetPlayer();*/
	player->KeyState(states);
}

int CPlayScene::GetTimer()
{
	timer = (int)(MAX_PLAY_TIME - (GetTickCount64() - startSceneTime) / 1000);
	return timer > 0 ? timer : 0;
}

vector<LPGAMEOBJECT> CPlayScene::GetActiveGameObjects()
{
	auto activeObjects = grid->GetActiveObjects();

	for (size_t i = 0; i < objects.size(); i++)
	{
		if ((objects[i]->isStatic && objects[i]->isActive) || objects[i]->tag == Tag::TAIL || objects[i]->tag == Tag::MOVING_PLATFORM)
		{
			//Check if objects already in active list
			if (find(activeObjects.begin(), activeObjects.end(), objects[i]) == activeObjects.end())
				activeObjects.push_back(objects[i]);
		}
			
	}

	return activeObjects;
}
