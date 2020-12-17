#pragma once

#include "GameObject.h"

class Item : public CGameObject
{
protected:
	float startX;
	float startY;

	float spawnTime;

	bool isSprouting;
	bool hasSproutCompleted;
	float sproutHeight;
	float sproutSpeed;	

public:
	Item();
	Item(float,float);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void OnSproutComplete();

	virtual void SproutOut();

	bool CheckIsSprouting() { return isSprouting; }


};

