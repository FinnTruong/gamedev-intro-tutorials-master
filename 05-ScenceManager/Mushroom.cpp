#include "Mushroom.h"
#include "Player.h"

Mushroom::Mushroom(float startX, float startY) : Item(startX,startY)
{
	this->SetAnimationSet(CAnimationSetDatabase::GetInstance()->Get(ANIMATION_SET_MUSHROOM));
	tag = Tag::MUSHROOM;
	x = startX;
	y = startY;
	isTrigger = true;
	sproutSpeed = MUSHROOM_SPROUT_SPEED;
	sproutHeight = MUSHROOM_SPROUT_HEIGHT;
}

void Mushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!isActive)
		left = right = left = bottom = 0;
	left = x;
	top = y;
	right = left + MUSHROOM_WIDTH;
	bottom = top + MUSHROOM_HEIGHT;
}

void Mushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Item::Update(dt, coObjects);

	if (isSprouting)
		return;

	vy += MUSHROOM_GRAVITY * dt;;
	if (vx == 0)
		vx = MUSHROOM_WALKING_SPEED;

	HandleCollision(dt, coObjects);
}


void Mushroom::Render()
{
	if (!isActive)
		return;

	animation_set->at(0)->Render(nx, x, y, 255);
	RenderBoundingBox();
}

void Mushroom::HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx *= -1;
		if (ny != 0) vy = 0;


	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}


void Mushroom::OnSproutComplete()
{
	Item::OnSproutComplete();
}

