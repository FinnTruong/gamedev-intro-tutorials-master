#pragma once

#include "GameObject.h"

#define BRICK_WIDTH		16
#define BRICK_HEIGHT	16

#define BRICK_ANI_IDLE	0
#define BRICK_ANI_EMPTY 1
#define BRICK_ANI_COIN	2

#define BRICK_STATE_NORMAL	0
#define BRICK_STATE_COIN	1

#define COIN_DURATION		5000

class Brick : public CGameObject
{
	float startTransformTime;
public:
	Brick();
	~Brick();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	void HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void OnOverlapped(LPGAMEOBJECT obj);
};

