#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

#define	BRICK_STATE_COLLISION 10


class QuestionMarkBrick : public CGameObject
{
public:
	QuestionMarkBrick(float x, float y);
	~QuestionMarkBrick();

	float initialYPos;
	bool hasCollided;

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};