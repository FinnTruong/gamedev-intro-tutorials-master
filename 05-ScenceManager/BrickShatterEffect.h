#pragma once
#include "Effect.h"

#define BRICK_SHATTER_DURATION 2000

#define BRICK_SHATTER_EFFECT_ANIMATION_SET	21

class BrickShatterEffect : public Effect
{
public:
	BrickShatterEffect(float x, float y, int nx, float vyMultiplier);
	~BrickShatterEffect();

	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
};

