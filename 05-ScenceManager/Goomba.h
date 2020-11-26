#pragma once
#include "GameObject.h"

#define GOOMBA_WALKING_SPEED 0.05f;

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 15
#define GOOMBA_BBOX_HEIGHT_DIE 9

#define GOOMBA_TIME_BEFORE_DISAPPEARED 1000

#define GOOMBA_STATE_WALKING 1
#define GOOMBA_STATE_DIE 2
#define GOOMBA_STATE_DIE_ONESHOT 3

#define GOOMBA_ANI_WALKING 0
#define GOOMBA_ANI_DIE 1
#define GOOMBA_ANI_ONESHOT 2

class Goomba : public CGameObject
{
	float deadTime;
	float timeElapsed;
	bool hasBeenSteppedOn, hasBeenAttacked;

public: 	
	Goomba();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	virtual void HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);

	void OnSteppedOn();
	void OnAttacked();
};