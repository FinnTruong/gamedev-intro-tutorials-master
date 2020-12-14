#pragma once

#include "GameObject.h"

#define WM_MARIO_MOVE_SPEED		0.05f

#define WM_MARIO_ANI_SMALL		0
#define WM_MARIO_ANI_BIG		1
#define WM_MARIO_ANI_RACCOON	2
#define WM_MARIO_ANI_FIRE		3


class WorldMapMario : public CGameObject
{

public:
	bool isMoving = false;
	float startMovingXPos = 0;
	float startMovingYPos = 0;

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();

	void Move(Vector2 direction);
};

