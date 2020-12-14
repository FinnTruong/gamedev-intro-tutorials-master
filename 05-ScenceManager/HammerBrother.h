#pragma once

#include "GameObject.h"

#define HAMMER_BROTHER_SPEED						0.015f
#define HAMMER_BROTHER_DISTANCE_WALK_BEFORE_TURN	8

class HammerBrother:public CGameObject
{
public:
	HammerBrother(float x, float y);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
};

