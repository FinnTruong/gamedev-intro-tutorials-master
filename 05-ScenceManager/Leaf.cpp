#include "Leaf.h"
#include "Utils.h"
#include <cmath>



Leaf::Leaf(float startX, float startY) : Item(startX,startY)
{
	this->SetAnimationSet(CAnimationSetDatabase::GetInstance()->Get(ANIMATION_SET_LEAF));
	SetActive(true);
	tag = Tag::LEAF;
	x = startX;
	y = startY;	
	start_x = startX;
	isTrigger = true;
	sproutSpeed = LEAF_SPROUT_SPEED;
	sproutHeight = LEAF_SPROUT_HEIGHT;
}

void Leaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Item::Update(dt, coObjects);
	if (isSprouting)
		return;
	vy = 0.026;

	float timeElapsed = GetTickCount64() - spawnTime;

	x = start_x + 20 * cos(0.004 * timeElapsed);
	nx = -2 * LEAF_VELOCITY * 20 * sin(0.004 * timeElapsed);
	x += dx;
	y += dy;

}

void Leaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = right = top = bottom = 0;
}

void Leaf::Render()
{
	if (!isActive)
		return;

	animation_set->at(0)->Render(nx, x, y, 255);
	RenderBoundingBox();
}

void Leaf::OnSproutComplete()
{
	Item::OnSproutComplete();
}

Leaf::~Leaf()
{

}

