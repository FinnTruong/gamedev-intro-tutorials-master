#pragma once
#include "GameObject.h"
#include "Item.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

#define	BRICK_STATE_COLLISION 10


class QuestionMarkBrick : public CGameObject
{
	float startY;
	bool hasCollided;
	bool hasItem = true;
	bool hasSpawn = false;

	Item* item = nullptr;

public:
	QuestionMarkBrick(float x, float y, bool hasItem = 0);
	~QuestionMarkBrick();


	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	void SpawnItem();
};