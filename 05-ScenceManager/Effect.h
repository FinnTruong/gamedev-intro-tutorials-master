#pragma once

#include "GameObject.h"

class Effect : public CGameObject
{
protected:
	ULONGLONG startTime;
	ULONGLONG duration;

	bool hasDone;
public:
	Effect();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
};

