#pragma once
#include "GameObject.h"

#define SLEDGE_BRO_WIDTH	16
#define SLEDGE_BRO_HEIGHT	24

#define SLEDGE_BRO_MOVE_SPEED 0.025
#define SLEDGE_BRO_MOVE_DISTANCE 24

class SledgeBro : public CGameObject
{
public:
	SledgeBro(float x, float y);
	~SledgeBro() {}
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

