#include "Brick.h"
#include "Utils.h"
#include "KoopaTroopa.h"

Brick::Brick()
{
	tag = Tag::BRICK;
	SetState(BRICK_STATE_NORMAL);
}

void Brick::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
	if (state == BRICK_STATE_COIN)
	{
		if (GetTickCount64() - startTransformTime > COIN_DURATION)
			SetState(BRICK_STATE_NORMAL);
	}

	HandleCollision(dt, coObjects);
}

void Brick::Render()
{
	if (!isActive)
		return;

	int ani = 0;
	if (state == BRICK_STATE_NORMAL)
		ani = BRICK_ANI_IDLE;
	else if (state == BRICK_STATE_COIN)
		ani = BRICK_ANI_COIN;

	animation_set->at(ani)->Render(1, x, y);

	RenderBoundingBox();
}

void Brick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == BRICK_STATE_COIN)
	{
		l = r = t = b = 0;
	}
	else
	{
		l = x;
		t = y;
		r = l + BRICK_WIDTH;
		b = t + BRICK_HEIGHT;
	}

}

void Brick::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BRICK_STATE_NORMAL:
		isTrigger = false;
		break;
	case BRICK_STATE_COIN:
		startTransformTime = GetTickCount64();
		isTrigger = true;
		break;
	default:
		break;
	}
}

void Brick::HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);
	CalcPotentialOverlapped(coObjects);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += (float)dx;
		y += (float)dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Brick::OnOverlapped(LPGAMEOBJECT obj)
{
	if (obj->tag == Tag::TAIL)
	{
		DebugOut(L"Hit");
		//Disable Brick
		SetActive(false);
		//Play Effect
	}
}