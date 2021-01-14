#pragma once

#include "GameObject.h"

#define WAIT_TIME	2200
#define FLASH_TIME	200
class Background : public CGameObject
{
	ULONGLONG startTime = 0;
public:
	Background();
	~Background();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
};

