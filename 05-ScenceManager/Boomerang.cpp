#include "Boomerang.h"
#include "Utils.h"
Boomerang::Boomerang(float posX, float posY)
{
	this->SetAnimationSet(CAnimationSetDatabase::GetInstance()->Get(BOOMERANG_ANIMATION_SET));
	tag = Tag::BOOMERANG;
	x = posX;
	y = posY;
	start_x = posX;
	start_y = posY;
	vy = -0.12;
	vx = BOOMERANG_MOVING_SPEED;
	boomerangStartTime = GetTickCount64();
	isStatic = true;
	isTrigger = true;
	sortingLayer = 2;
}

void Boomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	/*left = x;
	top = y;
	right = x + BOOMERANG_WIDTH;
	bottom = y + BOOMERANG_HEIGHT;*/
	left = right = top = bottom = 0;
}

void Boomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);

	x += dx;
	y += dy;

	if (vx > 0)
	{
		vy += 0.0002 * dt;
		if (y >= start_y - 4)
		{
			vx -= 0.00002 * dt;
			if (vx <= BOOMERANG_MOVING_SPEED)
				vx = -BOOMERANG_MOVING_SPEED;
		}
	}

	if (vx < 0)
	{
		vy -= 0.0002 * dt;
		if (y <= start_y + 4)
		{
			vx += 0.00002 * dt;
			if (vx >= -BOOMERANG_MOVING_SPEED)
				vx = -BOOMERANG_MOVING_SPEED;
		}
	}

	if (x - start_x >= BOOMERANG_TRAVEL_DISTANCE && vx > 0)
	{
		vy = 0.1;
		/*vx -= 0.00002 * dt;
		if (vx <= BOOMERANG_MOVING_SPEED)
			vx = -BOOMERANG_MOVING_SPEED;*/
	}
	else if (x <= start_x && vx < 0)
	{
		vy = -0.1;
		vx = BOOMERANG_MOVING_SPEED;
	}

	if (GetTickCount64() - boomerangStartTime >= BOOMERANG_DURATION)
	{
		SetActive(false);
		DisableGameObject();
		return;
	}
}

void Boomerang::Render()
{
	DebugOut(L"Render \n");
	animation_set->at(0)->Render(1, x, y);
	RenderBoundingBox();
}
