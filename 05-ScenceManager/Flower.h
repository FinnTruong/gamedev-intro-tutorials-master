#pragma once

#include "Item.h"

#define ANIMATION_SET_FLOWER	24

#define FLOWER_WIDTH	16
#define FLOWER_HEIGHT	16

#define FLOWER_SPROUT_SPEED 0.05f
#define FLOWER_SPROUT_HEIGHT 15



class Flower : public Item
{
public:
	Flower(float x, float y);
	~Flower();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt);
	void Render();
};

