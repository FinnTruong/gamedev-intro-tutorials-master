#pragma once

#include "GameObject.h"

#define KOOPA_WALKING_SPEED 0.03f;

#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 25
#define KOOPA_BBOX_HEIGHT_DIE 16

#define KOOPA_ANI_WALKING		0
#define KOOPA_ANI_SHELL			1
#define KOOPA_ANI_SPIN			2
#define KOOPA_ANI_DIE			3

#define KOOPA_STATE_WALKING			0
#define KOOPA_STATE_SHELL			1
#define KOOPA_STATE_SPIN			2
#define KOOPA_STATE_DIE				3


class Koopa : public CGameObject
{
	bool hasBeenSteppedOn, hasBeenAttacked;
public:
	Koopa();

	bool isBeingHeld;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(int state);

	void OnSteppedOn();
	void OnAttacked();
};