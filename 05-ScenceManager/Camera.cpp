#include "Camera.h"
#include "Game.h"
#include "Player.h"
#include "PlayScence.h"
#include "Tilemap.h"
////Camera* Camera::GetInstance()
//{
//	if (instance == NULL)
//		instance = new Camera();
//	return instance;
//}

Camera::Camera()
{
	x = 0;
	y = 0;
}

void Camera::Update(DWORD dt)
{
	FollowPlayer(dt, x, y);

	if (Mario->isFlying)
		cameraFolowYAxis = true;
	else if (!Mario->isFlying && y >= 64)
		cameraFolowYAxis = false;

	SetPosition(x, !Mario->inSecretRoom ?  y : 286);
}

void Camera::FollowPlayer(DWORD dt, float &camX, float &camY)
{
	Mario->GetPosition(camX, camY);

	CGame* game = CGame::GetInstance();

	camX -= game->GetScreenWidth() / 2;
	camY -= game->GetScreenHeight() / 3;

	auto tilemap = dynamic_cast<CPlayScene*>(game->GetCurrentScene())->GetTilemap();

	//Check Boundaries
	if (camX <= 0)
	{
		camX = 0;
	}
	else if (camX + SCREEN_WIDTH >= tilemap->GetWidthTileMap())
	{
		camX = (float)tilemap->GetWidthTileMap() - SCREEN_WIDTH;
	}


	if (!Mario->hasHitGoal)
	{
		if (Mario->x < camX)
			Mario->x = camX;
		else if (Mario->x + 16 >= camX + SCREEN_WIDTH - 16)
			Mario->x = camX + SCREEN_WIDTH - 32;
	}

	if (cameraFolowYAxis)
	{
		if (camY >= 64)
		{
			camY = 64;
		}
		else if (camY < PLAY_TILEMAP_Y_OFFSET)
		{
			camY = PLAY_TILEMAP_Y_OFFSET;
		}
	}
	else
		camY = 64;

}

void Camera::AutoMove(DWORD dt)
{
	CGameObject::Update(dt);
	x += dx;
	y += dy;

	if (!cameraFollowXAxis)
	{
		if (x + SCREEN_WIDTH <= 2064)
			vx = CAMERA_MOVE_SPEED;
		else
			vx = 0;
	}
	else
	{
		Mario->GetPosition(x, y);

		CGame* game = CGame::GetInstance();

		x -= game->GetScreenWidth() / 2;
		y = 64;

		auto tilemap = dynamic_cast<CPlayScene*>(game->GetCurrentScene())->GetTilemap();


		//Check Boundaries
		if (x <= 2080)
		{
			x = 2080;
		}
		else if (x + SCREEN_WIDTH >= tilemap->GetWidthTileMap())
		{
			x = (float)tilemap->GetWidthTileMap() - SCREEN_WIDTH;
		}	

		if (!Mario->hasHitGoal)
		{
			if (Mario->x < x)
				Mario->x = x;
			else if (Mario->x + 16 >= x + SCREEN_WIDTH - 16)
				Mario->x = x + SCREEN_WIDTH - 32;
		}
	}

}


Rect Camera::GetRect()
{
	return Rect(x, y, SCREEN_WIDTH, SCREEN_HEIGHT);
}
