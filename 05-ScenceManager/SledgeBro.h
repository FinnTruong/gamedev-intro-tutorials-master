#pragma once
#include "GameObject.h"

class SledgeBro : public CGameObject
{
public:
	
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
};

