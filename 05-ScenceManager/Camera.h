#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "GameGlobal.h"
class Camera
{
	int cam_x, cam_y;
public:
	Camera();
	~Camera();

	void Update();
	void SetPosition(int posX, int posY) { cam_x = posX; cam_y = posY; }
	void GetPosition(int& posX, int& posY) { posX = cam_x; posY = cam_y; }

	Vector2 ViewportToWorld(float& x, float& y) { return Vector2((float)(x + cam_x), (float)(y + cam_y)); }
};