#pragma once

#include "Projectile.h"

#define BOOMERANG_ANIMATION_SET	25

#define BOOMERANG_WIDTH		16
#define BOOMERANG_HEIGHT	16

#define BOOMERANG_TRAVEL_DISTANCE	128
#define BOOMERANG_MOVING_SPEED		0.1f

#define BOOMERANG_DURATION	2000

class Boomerang : public Projectile
{
	ULONGLONG boomerangStartTime = 0;

	bool isTurningBack = false;
public:
	Boomerang(float x, float y);
	~Boomerang() {}

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	void Render();

};

