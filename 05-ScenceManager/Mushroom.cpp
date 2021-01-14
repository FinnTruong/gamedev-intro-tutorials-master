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
		vx = moveDirection * MUSHROOM_WALKING_SPEED;

	HandleCollision(dt, coObjects);
}


void Mushroom::Render()
{
	if (!isActive)
		return;
	
	int ani = 0;

	if (tag == Tag::MUSHROOM)
		ani = MUSHROOM_ANI_NORMAL;
	else if (tag == Tag::ONE_UP_MUSHROOM)
		ani = MUSHROOM_ANI_ONE_UP;
	
	animation_set->at(ani)->Render(nx, x, y, 255);

	RenderBoundingBox();
}

void Mushroom::HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);
	CalcPotentialOverlapped(coObjects);

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
	moveDirection = x >= Mario->x ? 1 : -1;
	Item::OnSproutComplete();
}

void Mushroom::OnOverlapped(LPGAMEOBJECT obj)
{
	if (obj->tag == Tag::PLAYER)
	{
		OnCollected();
	}
}

