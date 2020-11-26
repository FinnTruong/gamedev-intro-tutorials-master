#include "Leaf.h"
#include "Utils.h"
#include <cmath>



Leaf::Leaf(float startX, float startY)
{
	this->SetAnimationSet(CAnimationSetDatabase::GetInstance()->Get(ANIMATION_SET_LEAF));
	SetActive(true);
	tag = Tag::ITEM;
	x = startX;
	y = startY;	
	start_x = startX;
	isTrigger = true;
	spawnTime = GetTickCount64();
	vx = LEAF_VELOCITY;
}

void Leaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	vy = 0.026;

	float timeElapsed = GetTickCount64() - spawnTime;

	x = start_x + 20 * cos(0.004 * timeElapsed);
	nx = -2 * LEAF_VELOCITY * 20 * sin(0.004 * timeElapsed);
	//x += dx;
	y += dy;
}

void Leaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 16;
	bottom = y + 16;
}

void Leaf::OnOverlapped(CGameObject* other)
{
	if (other->tag == Tag::PLAYER)
	{
		DebugOut(L"Overlapped");
		this->SetActive(false);
	}
}

void Leaf::HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

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

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;
			LPCOLLISIONEVENT e = coEventsResult[i];

		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Leaf::Render()
{
	if (!isActive)
		return;

	animation_set->at(0)->Render(nx, x, y, 255);
	RenderBoundingBox();
}

