#pragma once

#include "GameObject.h"

#define MARIO_WIDTH 14

class Player : public CGameObject
{
protected:
	float vx;
public:
	Player(float x, float y, float vx);
	void Update(DWORD dt);
	void Render();
};
