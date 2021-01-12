#pragma once

#include "GameObject.h"

#define MOVING_PLATFORM_WIDTH 32.f
#define MOVING_PLATFORM_HEIGHT 16.f

class MovingPlatform : public CGameObject
{
	float moveSpeed;
	float fallSpeed;
public:
	MovingPlatform();
	~MovingPlatform();

	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

