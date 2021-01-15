#include "Flower.h"

Flower::Flower(float startX, float startY) : Item(startX, startY)
{
	this->SetAnimationSet(CAnimationSetDatabase::GetInstance()->Get(ANIMATION_SET_FLOWER));
	tag = Tag::FLOWER;
	x = startX;
	y = startY;
	isTrigger = true;
	sproutSpeed = FLOWER_SPROUT_SPEED;
	sproutHeight = FLOWER_SPROUT_HEIGHT;
}

void Flower::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!isActive)
		left = right = left = bottom = 0;
	left = x;
	top = y;
	right = left + FLOWER_WIDTH;
	bottom = top + FLOWER_HEIGHT;
}

void Flower::Render()
{
	animation_set->at(0)->Render(1, x, y);
}