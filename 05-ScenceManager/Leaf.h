#pragma once

#include "GameObject.h"


#define LEAF_GRAVITY 0.0002f
#define LEAF_VELOCITY 0.1

#define LEAF_ANI_FALL 0
#define ANIMATION_SET_LEAF	10

class Leaf : public CGameObject
{
	float start_x;
	float spawnTime;
public:
	Leaf(float x, float y);
	~Leaf();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void OnOverlapped(CGameObject* other);
	virtual void SetState(int state) {}
	
};

