#pragma once

#include "GameObject.h"

#define GOAL_ANI_RANDOM			0
#define GOAL_ANI_MUSHROOM		1
#define GOAL_ANI_FLOWER			2
#define GOAL_ANI_STAR			3

#define GOAL_MUSHROOM		1
#define GOAL_FLOWER			2
#define GOAL_STAR			3

#define GOAL_STATE_RANDOM		0
#define GOAL_STATE_HIT			1

#define TIME_BETWEEN_SWITCH		100

class Goal : public CGameObject
{
	int curType = 0;
	bool hasTouch = false;

	ULONGLONG startTypeTime = 0;
public:
	Goal();
	~Goal();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int level);
};

