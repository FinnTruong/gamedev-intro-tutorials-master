#include "Brick.h"
#include "Utils.h"
#include "KoopaTroopa.h"
#include "BrickShatterEffect.h"

Brick::Brick()
{
	tag = Tag::BRICK;
	width = BRICK_WIDTH;
	height = BRICK_HEIGHT;
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
	if (state == BRICK_STATE_COIN || state == BRICK_STATE_SHATTER)
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
	case BRICK_STATE_SHATTER:
		//Disable Brick
		SetActive(false);
		DisableGameObject();
		auto topLeftPiece = new BrickShatterEffect(x - 1, y - 2, -1, 2.f);
		auto topRightPiece = new BrickShatterEffect(x + 9, y - 2, 1, 2.f);
		auto bottomLeftPiece = new BrickShatterEffect(x - 1, y + 8, -1, 1.f);
		auto bottomRightPiece = new BrickShatterEffect(x + 9, y + 8, 1, 1.f);
		break;
	}
}

void Brick::HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CalcPotentialOverlapped(coObjects);
}

void Brick::OnOverlapped(LPGAMEOBJECT obj)
{
	if (obj->tag == Tag::TAIL)
	{
		if (state != BRICK_STATE_SHATTER)
			SetState(BRICK_STATE_SHATTER);
	}
}