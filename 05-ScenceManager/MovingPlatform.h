#pragma once

#include "GameObject.h"

#define MOVING_PLATFORM_WIDTH 48
#define MOVING_PLATFORM_HEIGHT 16

#define MOVING_PLATFORM_MOVE_SPEED	-0.035f
#define MOVING_PLATFORM_FALL_SPEED	0.0002f
#define MOVING_PLATFORM_TERMINAL_VELOCITY	0.15f

#define TIME_BEFORE_FALLING		100

#define CAMERA_RANGE_OFFSET		8

class MovingPlatform : public CGameObject
{
public:

	bool hasTouched;
	ULONGLONG playerTouchTime;

	MovingPlatform();
	~MovingPlatform();

	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	bool InCameraRange();
};

