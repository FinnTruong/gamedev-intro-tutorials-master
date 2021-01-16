#pragma once
#include "GameObject.h"

#define SLEDGE_BRO_WIDTH	16
#define SLEDGE_BRO_HEIGHT	24

#define SLEDGE_BRO_MOVE_SPEED 0.025
#define SLEDGE_BRO_MOVE_DISTANCE 24

#define MIN_DISTANCE_FROM_PLAYER	80

#define TIME_BETWEEN_ATTACK		1000

class SledgeBro : public CGameObject
{
	bool canAttack = false;
	ULONGLONG attackTime = 0;
public:
	SledgeBro(float x, float y);
	~SledgeBro() {}

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void Attack();

	bool IsPlayerInRange();
};

