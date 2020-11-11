#include "Fireball.h"

Fireball::Fireball()
{
	this->SetAnimationSet(CAnimationSetDatabase::GetInstance()->Get(ANIMATION_SET_FIREBALL));
	x = 0;
	y = 0;
	alpha = 0;
	isCollision = 0;
	isDone = true;
	damage = 1;
	timeDelayed = 0;
	timeDelayMax = FIREBALL_DELAY;
}

Fireball::~Fireball() {}

void Fireball::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (isDone == true)
	{
		alpha = 0;
	}
	else
	{
		vy += FIREBALL_GRAVITY * dt;
		//DebugOut(L"direction %d", direction);
		vx = FIREBALL_SPEED * nx;
		timeDelayed += dt;
		CGameObject::Update(dt);

#pragma region Xử lý va chạm
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		CalcPotentialCollisions(colliable_objects, coEvents);

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
				if (e->nx != 0)
				{
					isCollision = 1;
					vx = 0;
					vy = 0;
				}
				if (e->ny != 0)
					vy = -FIREBALL_DEFLECT_SPEED_Y;
			}
		}
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
	}
}

void Fireball::Render()
{
	RenderBoundingBox();
	int ani;
	if (timeDelayed >= timeDelayMax)
	{
		isDone = true;
		timeDelayed = 0;
	}
	else
	{
		if (!isCollision)
		{
			ani = FIREBALL_ANI_FIRE;
			animation_set->at(0)->Render(nx, x, y, alpha);
		}
		else
		{
			ani = FIREBALL_ANI_BANG;
			animation_set->at(ani)->Render(nx, x, y - DISTANCE_TO_BANG, alpha);
			if (animation_set->at(ani)->GetCurrentFrame() == 4)
			{
				isDone = true;
				timeDelayed = 0;
			}
		}
	}
}

void Fireball::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + FIREBALL_BBOX_HEIGHT;
	b = y + FIREBALL_BBOX_WIDTH;
}