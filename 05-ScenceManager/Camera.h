#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "GameGlobal.h"
class Camera
{
	float cam_x, cam_y;

	float camSpeed = 0.0f;
public:
	Camera();
	~Camera();

	bool cameraFolowYAxis = false;

	void Update(DWORD dt);
	void CheckBoundaries(DWORD dt, float&, float&);
	void SetPosition(float posX, float posY) { cam_x = posX; cam_y = posY; }
	void GetPosition(int& posX, int& posY) { posX = (int)cam_x; posY = (int)cam_y; }
	void SmoothFollow(DWORD dt, float targetX, float targetY, float& x, float& y);

	Vector2 ViewportToWorld(float& x, float& y) { return Vector2((float)(x + cam_x), (float)(y + cam_y)); }
};