#pragma once
#include "Item.h"

#define MUSHROOM_GRAVITY 0.0002f
#define MUSHROOM_WALKING_SPEED 0.05f
#define MUSHROOM_SPROUT_SPEED 0.05f
#define MUSHROOM_SPROUT_HEIGHT 20

#define MUSHROOM_WIDTH 16
#define MUSHROOM_HEIGHT 16

#define ANIMATION_SET_MUSHROOM	11

#define MUSHROOM_ANI_NORMAL		0
#define MUSHROOM_ANI_ONE_UP		1

class Mushroom : public Item
{
	ULONGLONG spawnTime = 0;
public:
	Mushroom(float x, float y);
	~Mushroom();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void OnSproutComplete();

	void OnOverlapped(LPGAMEOBJECT obj);
};
