#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "GameGlobal.h"
#include "GameObject.h"

#define CAMERA_MOVE_SPEED 0.045f
class Camera : public CGameObject
{

	float camSpeed = 0.0f;
public:
	Camera();
	~Camera();

	bool cameraFolowYAxis = false;
	bool cameraFollowXAxis = false;

	void Update(DWORD dt);
	void FollowPlayer(DWORD dt, float&, float&);
	void AutoMove(DWORD dt);
	void SetPosition(float posX, float posY) { x = posX; y = posY; }
	void GetPosition(int& posX, int& posY) { posX = (int)x; posY = (int)y; }

	Vector2 ViewportToWorld(float& posX, float& posY) { return Vector2((float)(posX + x), (float)(posY + y)); }

	Rect GetRect();
};