#pragma once

#include "GameObject.h"


#define TAIL_WIDTH 10
#define TAIL_HEIGHT 8

#define TAIL_PHASE_TIME 150

class Tail : public CGameObject
{
	ULONGLONG firstPhaseStartTime = 0;
	ULONGLONG secondPhaseStartTime = 0;
	ULONGLONG thirdPhaseStartTime = 0;
public:
	Tail();
	~Tail();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();

	void Attack(float x, float y);

};

