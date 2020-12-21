#pragma once

#include "Effect.h"

#define EXPLODE_EFFECT_ANIMATION_SET	22

#define EXPLODE_EFFECT_DURATION 500

class ExplodeEffect : public Effect
{
public:
	ExplodeEffect();
	~ExplodeEffect();

	void Render();
};

