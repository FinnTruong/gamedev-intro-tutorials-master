#pragma once

#include "GameObject.h"
#include "Item.h"


#define LEAF_GRAVITY 0.0002f
#define LEAF_VELOCITY 0.1
#define LEAF_SPROUT_SPEED 0.05f
#define LEAF_SPROUT_HEIGHT 50

#define LEAF_ANI_FALL 0
#define ANIMATION_SET_LEAF	10

class Leaf : public Item
{
	float start_x;
	float spawnTime;
public:
	Leaf(float x, float y);
	~Leaf();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void OnSproutComplete();
	
};

