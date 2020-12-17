#pragma once

#include "Effect.h"

#define POINT_EFFECT_ANIMATION_SET	20

#define POINT_ANI_100	0
#define POINT_ANI_200	1
#define POINT_ANI_400	2
#define POINT_ANI_800	3
#define POINT_ANI_1000	4
#define POINT_ANI_2000	5
#define POINT_ANI_4000	6
#define POINT_ANI_8000	7
#define POINT_ANI_1UP	8

#define POINT_EFFECT_DURATION		300
#define POINT_EFFECT_MOVE_SPEED		-0.07f

class PointEffect : public Effect
{
	int point;
public:
	PointEffect(float x, float y, int point);

	void Render();
};


