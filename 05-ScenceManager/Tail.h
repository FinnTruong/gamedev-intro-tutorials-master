#pragma once

#include "GameObject.h"


#define TAIL_WIDTH 10
#define TAIL_HEIGHT 8

#define TAIL_PHASE_TIME 150

class Tail : public CGameObject
{
	float firstPhaseStartTime;
	float secondPhaseStartTime;
	float thirdPhaseStartTime;
public:
	Tail();
	~Tail();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();

	void Attack(float x, float y);

};

