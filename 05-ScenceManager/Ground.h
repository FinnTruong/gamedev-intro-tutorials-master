#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

class Ground : public CGameObject
{
	int width, height;
public:
	Ground(int width = 16, int height = 16);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
