#pragma once
#include "GameObject.h"
#include "Item.h"

#define COIN_ANI_IDLE		0
#define ANIMATION_SET_COIN	12

#define COIN_GRAVITY	0.002
#define COIN_SPROUTING_SPEED -0.015

class Coin : public Item
{
	bool hasPointEffect;
public:
	Coin(float x, float y);
	~Coin();

	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Render();

	void SproutOut();

};

