#include "Item.h"
#include "Utils.h"
#include "Leaf.h"

Item::Item(float x,float y)
{
	startX = x;
	startY = y;	
}

void Item::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	if (isSprouting)
	{
		y -= sproutSpeed * dt;		
	}
	float delta = startY - y;
	if (delta >= sproutHeight && !hasSproutCompleted)
	{
		OnSproutComplete();
	}
}

void Item::SproutOut()
{
	isSprouting = true;
}

void Item::OnSproutComplete()
{
	DebugOut(L"Sprout Complete");
	spawnTime = GetTickCount64();
	hasSproutCompleted = true;
	isSprouting = false;
}
