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
	cam_x = 0;
	cam_y = 0;
}

void Camera::Update(DWORD dt)
{
	CheckBoundaries(dt, cam_x, cam_y);

	if (Mario->isFlying)
		cameraFolowYAxis = true;
	else if (!Mario->isFlying && cam_y >= 64)
		cameraFolowYAxis = false;

	SetPosition((int)cam_x, !Mario->inSecretRoom ?  cam_y : 286);
}

void Camera::CheckBoundaries(DWORD dt, float &camX, float &camY)
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

void Camera::SmoothFollow(DWORD dt, float targetX, float targetY, float& x, float &y)
{
	float lerp = 0.001f;
	y = (y - targetY) * lerp * dt;
}
