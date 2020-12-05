#include "Fireball.h"
#include "Goomba.h"

Fireball::Fireball()
{
	this->SetAnimationSet(CAnimationSetDatabase::GetInstance()->Get(ANIMATION_SET_FIREBALL));
	x = 0;
	y = 0;
	alpha = 0;
	hasCollided = false;
	isDone = true;
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
		CGameObject::Update(dt);
		HandleCollision(dt, colliable_objects);
		if (!hasCollided)
		{
			vy += FIREBALL_GRAVITY * dt;
			vx = FIREBALL_SPEED * nx;
		}

		timeDelayed += dt;
	}
}



void Fireball::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + FIREBALL_BBOX_HEIGHT;
	b = y + FIREBALL_BBOX_WIDTH;

	if (hasCollided)
	{
		l = r = t = b = 0;
	}
}

void Fireball::HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
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
				hasCollided = true;
				vx = 0;
				vy = 0;
			}
			if (e->ny != 0)
				vy = -FIREBALL_DEFLECT_SPEED_Y;

			if (e->obj->tag == Tag::GOOMBA)
			{
				Goomba* goomba = dynamic_cast<Goomba*>(e->obj);
				if (e->nx != 0 || e->ny!= 0 )
				{
					hasCollided = true;
					//vy = -FIREBALL_DEFLECT_SPEED_Y;
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->OnAttacked();
					}
				}

			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
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
		if (!hasCollided)
		{
			ani = FIREBALL_ANI_FIRE;
			animation_set->at(0)->Render(nx, x, y, alpha);
		}
		else
		{
			ani = FIREBALL_ANI_BANG;
			animation_set->at(ani)->Render(nx, x, y - DISTANCE_TO_BANG, alpha);
			if (animation_set->at(ani)->HasAnimationEnded())
			{
				isDone = true;
				timeDelayed = 0;
			}
		}
	}
}

