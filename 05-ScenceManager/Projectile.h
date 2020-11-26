#pragma once

#include "GameObject.h"

class Projectile : public CGameObject
{
public:
	bool isDone;
	int damage;
	int isTargetTop;
	int alpha;
	bool hasCollided;
	float timeDelayed, timeDelayMax;

public:
	Projectile();
	~Projectile();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	void Fire(int direction, float posX, float posY) { nx = direction; x = posX; y = posY; alpha = 255; isDone = false; hasCollided = 0; }
	void ResetDelay() { timeDelayed = 0; }
	bool GetIsDone() { return isDone; }
	void SetIsDone(bool isdone) { isDone = isdone; }
	int GetDamage() { return damage; }
};

