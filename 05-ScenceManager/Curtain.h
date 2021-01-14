#pragma once
#include "GameObject.h"

#define CURTAIN_MOVE_SPEED -0.2f

#define CURTAIN_MOVE_TIME	500

#define TIME_TO_WAIT	1000

class Curtain: public CGameObject
{
	ULONGLONG startTime = 0;
public:
	Curtain();
	~Curtain();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
};

