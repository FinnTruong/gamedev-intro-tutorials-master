#pragma once

#include "GameObject.h"

#define PIRANHA_WIDTH 16
#define PIRANHA_HEIGHT 24

#define PIRANHA_STATE_RISE		0
#define PIRANHA_STATE_ATTACK	1
#define PIRANHA_STATE_SINK		2

#define PIRANHA_ANI_BITTING		0

#define PIRANHA_SPEED			0.05f
#define PIRANHA_ATTACK_TIME		1000
#define PIRANHA_COOLDOWN_TIME	1000

#define PIRANHA_MIN_DISTANCE_TO_PLAYER	32
#define PIRANHA_MAX_DISTANCE_TO_PLAYER	128

class PiranhaPlant : public CGameObject
{
public:
	float plantHeight;

	ULONGLONG attackTime;
	ULONGLONG cooldownTime;

	bool isMoving;
	bool isCoolingDown;

	bool hasAttacked;
	bool canDamagePlayer = false;


public:
	PiranhaPlant(float, float);
	~PiranhaPlant();



	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {}
	virtual void SetState(int state);

	virtual void Attack();
	bool IsPlayerInRange();

};

