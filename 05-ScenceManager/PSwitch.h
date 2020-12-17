#pragma once

#include "Item.h"

#define SWITCH_WIDTH				16
#define SWITCH_NORMAL_HEIGHT		16
#define SWITCH_ACTIVATE_HEIGHT		7


#define SWITCH_STATE_NOT_ACTIVATE	0
#define SWITCH_STATE_ACTIVATE		1

#define SWITCH_ANIMATION_SET		17

#define SWITCH_ANI_NOT_ACTIVATE		0
#define SWITCH_ANI_ACTIVATE			1

#define P_SWITCH_SPROUT_SPEED		0.05f
#define P_SWITCH_SPROUT_HEIGHT		16

class PSwitch : public Item
{
public:
	PSwitch(float x, float y);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void SetState(int state);

};

