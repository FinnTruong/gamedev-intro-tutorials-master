#include "WorldSelectionScene.h"

WorldSelectionScene::WorldSelectionScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{

}
#pragma region  PARSE GAME DATA

void WorldSelectionScene::_ParseSection_TEXTURES(string line)
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

void WorldSelectionScene::_ParseSection_SPRITES(string line)
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

void WorldSelectionScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (auto i = 1; (unsigned)i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void WorldSelectionScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (auto i = 1; (unsigned)i < tokens.size(); i++)
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
void WorldSelectionScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

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
		obj = new WorldMapMario();
		player = (WorldMapMario*)obj;
		break;
	case OBJECT_TYPE_CACTUS: obj = new Cactus(); break;
	case OBJECT_TYPE_HAMMER_BROTHER: obj = new HammerBrother(x,y); break;
	case OBJECT_TYPE_SPEECH_BUBBLE: obj = new SpeechBubble(); break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x , y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}

void WorldSelectionScene::_ParseSection_TILEMAP(string line)
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
	//int widthGrid = atoi(tokens[9].c_str());
	//int heightGrid = atoi(tokens[10].c_str());
	//boardscore = new BoardScore();
	//grid = new Grid();
	//grid->Resize(widthGrid, heightGrid);
	//grid->PushGrid(allObject);
	tilemap = new Tilemap(ID, filePath_texture.c_str(), filePath_data.c_str(), num_row_on_texture, num_col_on_texture, num_row_on_tilemap, num_col_on_tilemap, tileset_width, tileset_height);

}

void WorldSelectionScene::_ParseSection_GRID(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1)
		return;

	wstring filePath_grid = ToWSTR(tokens[0]);

	LoadGrid(filePath_grid);
}

#pragma endregion

void WorldSelectionScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

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
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_TILEMAP: _ParseSection_TILEMAP(line); break;
		case SCENE_SECTION_GRID: _ParseSection_GRID(line); break;
		default: break;
		}
	}

	f.close();

	CTextureDatabase::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	CTextureDatabase::GetInstance()->Add(ID_TEX_HUD_BG, L"textures\\UI\\squarebox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void WorldSelectionScene::Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;

	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		//objects[i]->CalcPotentialCollisions(&coObjects, objects[i]->coEvents);
		objects[i]->Update(dt, &coObjects);
	}

}

void WorldSelectionScene::Render()
{
	tilemap->Draw(WORLD_MAP_TILEMAP_X_OFFSET, WORLD_MAP_TILEMAP_Y_OFFSET);

	//Render objects
	for (auto i = 0; (unsigned)i < objects.size(); i++)
	{
		objects[i]->Render();
	}

	hud->Draw();
}

/*
	Unload current scene
*/
void WorldSelectionScene::Unload()
{
	for (auto i = 0; (unsigned)i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	mapGrid.clear();
	hasLoadGrid = false;

	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void WorldSelectionScene::OnKeyDown(int key)
{
	keyCode[key] = true;
	if (player->isMoving)
		return;
	switch (key)
	{

	case DIK_UP:
		if (CheckIfCanMove(Vector2(0, 1)))
		{
			player->Move(Vector2(0, 1));
			curNodeX -= 2;
			DebugOut(L"current node: [%d][%d]", curNodeX, curNodeY);
			DebugOut(L"current node value: %d", mapGrid[curNodeX][curNodeY]);
		}
		break;
	case DIK_DOWN:
		if (CheckIfCanMove(Vector2(0, -1)))
		{
			player->Move(Vector2(0, -1));
			curNodeX += 2;
			DebugOut(L"current node: [%d][%d]", curNodeX, curNodeY);
			DebugOut(L"current node value: %d", mapGrid[curNodeX][curNodeY]);
		}
		break;
	case DIK_LEFT:
		if (CheckIfCanMove(Vector2(-1, 0)))
		{
			player->Move(Vector2(-1, 0));
			curNodeY -= 2;
			DebugOut(L"current node: [%d][%d]", curNodeX, curNodeY);
			DebugOut(L"current node value: %d", mapGrid[curNodeX][curNodeY]);
		}
		break;
	case DIK_RIGHT:
		if (CheckIfCanMove(Vector2(1, 0)))
		{
			player->Move(Vector2(1, 0));
			curNodeY += 2;
			DebugOut(L"current node: [%d][%d]", curNodeX, curNodeY);
			DebugOut(L"current node value: %d", mapGrid[curNodeX][curNodeY]);
		}
		break;
	case DIK_J:
		if (mapGrid[curNodeX][curNodeY] == 3)
			CGame::GetInstance()->SwitchScene(1);
		else if (mapGrid[curNodeX][curNodeY] == 5)
			CGame::GetInstance()->SwitchScene(2);
	default:
		break;
	}
}

void WorldSelectionScene::OnKeyUp(int key)
{
	keyCode[key] = false;
}

bool WorldSelectionScene::CheckIfCanMove(Vector2 dir)
{
	if (dir.x == 1)
	{
		if (curNodeY + 1 > grid_columns - 1)
			return false;
		else if (mapGrid[curNodeX][curNodeY + 1] == 1)
			return false;
		else
			return true;
	}
	else if (dir.x == -1)
	{
		if (curNodeY - 1 < 0)
			return false;
		else if (mapGrid[curNodeX][curNodeY - 1] == 1)
			return false;
		else
			return true;
	}

	if (dir.y == 1)
	{
		if (curNodeX - 1 < 0)
			return false;
		else if (mapGrid[curNodeX - 1][curNodeY] == 1)
			return false;
		else
			return true;
	}
	else if (dir.y == -1)
	{
		if (curNodeX + 1 > grid_rows - 1)
			return false;
		else if (mapGrid[curNodeX + 1][curNodeY] == 1)
			return false;
		else
			return true;
	}
	return false;
}

void WorldSelectionScene::LoadGrid(wstring filePath)
{
	if (hasLoadGrid)
		return;


	ifstream fs(filePath, ios::in);
	fs >> grid_rows >> grid_columns;

	vector<vector<int>> grid(grid_rows, vector<int>(grid_columns, 0));

	for (auto i = 0; i < grid_rows; i++)
	{
		for (auto j = 0; j < grid_columns; j++)
		{
			fs >> grid[i][j];
		}
	}

	mapGrid.clear();
	mapGrid = grid;

	fs.close();

	hasLoadGrid = true;

	//Find Start Node

	for (auto i = 0; i < grid_rows; i++)
	{
		for (auto j = 0; j < grid_columns; j++)
		{
			if (mapGrid[i][j] == MapNode::START)
			{
				curNodeX = i;
				curNodeY = j;
			}

		}
	}
}