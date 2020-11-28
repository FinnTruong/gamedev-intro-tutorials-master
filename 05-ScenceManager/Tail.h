#pragma once

#include "GameObject.h"


#define TAIL_WIDTH 10
#define TAIL_HEIGHT 9

class Tail : public CGameObject
{
public:
	Tail();
	~Tail();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();

	void Attack(float x, float y);

};

