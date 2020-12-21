#pragma once
#include "GameObject.h"
#include "Item.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

#define	QUESTION_BLOCK_STATE_HIT_FROM_BOTTOM		0
#define	QUESTION_BLOCK_STATE_HIT_FROM_SIDE			1


class QuestionBlock : public CGameObject
{
	float startY;
	bool hasCollided;
	bool hasPowerUp = false;
	bool hasSpawn = false;

	Item* item = nullptr;

public:
	QuestionBlock(float x, float y, bool hasItem = 0);
	~QuestionBlock();


	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	void OnOverlapped(LPGAMEOBJECT obj);
	void SpawnItem();
};