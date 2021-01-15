#include "MovingPlayScene.h"

MovingPlayScene::MovingPlayScene(int id, LPCWSTR filePath) : CPlayScene(id, filePath)
{
	sceneType = 3;
	camera->y = 64;
}

void MovingPlayScene::Update(DWORD dt)
{
	int cam_x, cam_y;
	camera->GetPosition(cam_x, cam_y);

	//Make sure the player doesn't exit the screen view if hasn't complete level
	if (!player->hasHitGoal)
	{
		if (player->x <= cam_x)
			player->x = (float)cam_x;
		if (player->y >= cam_y + SCREEN_HEIGHT)
			player->isDead = true;
	}

	

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

	camera->AutoMove(dt);
}

void MovingPlayScene::Render()
{
	tilemap->Draw(MOVING_SCENE_TILEMAP_X_OFFSET, MOVING_SCENE_TILEMAP_Y_OFFSET);
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

void MovingPlayScene::Load()
{
	CPlayScene::Load();
	camera->x = 0;
}

void MovingPlayScene::DrawEndLevelText()
{
	CPlayScene::DrawEndLevelText();
}
