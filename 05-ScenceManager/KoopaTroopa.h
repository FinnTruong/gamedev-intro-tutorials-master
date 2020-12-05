#pragma once

#include "GameObject.h"

#define KOOPA_WALKING_SPEED 0.03f;
#define KOOPA_JUMPING_SPEED	0.25f;

#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 25
#define KOOPA_BBOX_HEIGHT_DIE 16

#define KOOPA_RED_ANI_JUMPING		0
#define KOOPA_RED_ANI_WALKING		1
#define KOOPA_RED_ANI_SHELL			2
#define KOOPA_RED_ANI_SPIN			3
#define KOOPA_RED_ANI_DIE			4
#define KOOPA_GREEN_ANI_JUMPING		5
#define KOOPA_GREEN_ANI_WALKING		6
#define KOOPA_GREEN_ANI_SHELL		7
#define KOOPA_GREEN_ANI_SPIN		8
#define KOOPA_GREEN_ANI_DIE			9

#define KOOPA_STATE_JUMPING			0
#define KOOPA_STATE_WALKING			1
#define KOOPA_STATE_SHELL			2
#define KOOPA_STATE_SPIN			3
#define KOOPA_STATE_DIE				4
#define KOOPA_STATE_DIE_ONESHOT		5


class KoopaTroopa : public CGameObject
{
protected:
	int type;

	bool isGrounded;
	bool hasBeenSteppedOn, hasBeenAttacked;
public:
	KoopaTroopa();
	~KoopaTroopa();

	bool isBeingHeld;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(int state);

	void OnSteppedOn();
	void OnAttacked();
};

enum KoopaTroopaType
{
	KOOPA_RED,
	KOOPA_GREEN
};