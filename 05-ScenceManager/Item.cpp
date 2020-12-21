#include "Item.h"
#include "Utils.h"
#include "Leaf.h"
#include "PointEffect.h"
#include "Game.h"

Item::Item(float x,float y)
{
	start_x = x;
	start_y = y;	
	sortingLayer = 0;
}

void Item::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);


	if (isSprouting)
	{
		y -= sproutSpeed * dt;		
	}
	float delta = start_y - y;
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
	sortingLayer = 1;
}

void Item::OnCollected()
{
	SetActive(false);
	DisableGameObject();
	auto pointEffect = new PointEffect(x, y, 1000);
	CGame::GetInstance()->AddScore(1000);
}