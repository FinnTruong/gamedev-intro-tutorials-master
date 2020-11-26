#include "Coin.h"

Coin::Coin(float startX, float startY)
{
	this->SetAnimationSet(CAnimationSetDatabase::GetInstance()->Get(12));
	SetActive(true);
	tag = Tag::ITEM;
	x = startX;
	y = startY;
	isTrigger = true;
}

void Coin::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 15;
	bottom = y + 15;
}

void Coin::Render()
{
	if (!isActive)
		return;

	animation_set->at(0)->Render(nx, x, y, 255);
	RenderBoundingBox();
}