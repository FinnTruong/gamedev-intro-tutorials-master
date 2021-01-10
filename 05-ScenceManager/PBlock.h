#pragma once

#include "GameObject.h"
#include "PSwitch.h"


#define P_BLOCK_WIDTH		16
#define P_BLOCK_HEIGHT		16

#define P_BLOCK_STATE_NORMAL	0
#define P_BLOCK_STATE_EMPTY	1

#define P_BLOCK_ANI_NORMAL	0
#define P_BLOCK_ANI_EMPTY	1

class PBlock : public CGameObject
{
	bool isEmpty, hasSpawn = false;
	Item* p_switch;
public:
	PBlock(float, float);
	~PBlock();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	void SpawnPSwitch();

	void OnOverlapped(LPGAMEOBJECT obj);
};

