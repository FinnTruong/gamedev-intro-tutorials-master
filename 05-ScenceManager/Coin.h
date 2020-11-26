#pragma once
#include "GameObject.h"

#define COIN_ANI_IDLE		0
#define ANIMATION_SET_COIN	12

class Coin : public CGameObject
{
public:
	Coin(float x, float y);
	~Coin();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();

};

