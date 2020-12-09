#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "GameGlobal.h"
class Camera
{
	float cam_x, cam_y;
public:
	Camera();
	~Camera();

	void Update();
	void SetPosition(float posX, float posY) { cam_x = posX; cam_y = posY; }
	void GetPosition(float& posX, float& posY) { posX = cam_x; posY = cam_y; }

	Vector2 ViewportToWorld(float& x, float& y) { return Vector2(x + cam_x, y + cam_y); }
};