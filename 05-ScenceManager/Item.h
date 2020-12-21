#pragma once

#include "GameObject.h"

class Item : public CGameObject
{
protected:
	ULONGLONG spawnTime = 0;

	bool isSprouting = false;
	bool hasSproutCompleted = false;
	float sproutHeight = 0;
	float sproutSpeed = 0;	

public:
	Item();
	Item(float,float);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void OnSproutComplete();

	virtual void SproutOut();

	bool CheckIsSprouting() { return isSprouting; }

	void OnCollected();


};

