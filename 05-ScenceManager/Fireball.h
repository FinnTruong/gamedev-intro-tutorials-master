#pragma once

#include "Projectile.h"
#include <math.h>

#define ANIMATION_SET_FIREBALL		9
#define FIREBALL_BBOX_WIDTH			8
#define FIREBALL_BBOX_HEIGHT		9

#define DISTANCE_TO_BANG			5
#define FIREBALL_ANI_FIRE			0
#define FIREBALL_ANI_BANG			1

#define FIREBALL_SPEED					0.15f
#define FIREBALL_DEFLECT_SPEED_Y		0.13f
#define FIREBALL_GRAVITY				0.0005f
#define FIREBALL_DELAY					1500



class Fireball : public Projectile
{
public:
	Fireball();
	~Fireball();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();
	void HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
};


