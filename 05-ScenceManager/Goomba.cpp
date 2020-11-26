#include "Goomba.h"
#include "Ground.h"

Goomba::Goomba()
{
	tag = Tag::GOOMBA;
	SetState(GOOMBA_STATE_WALKING);
}

void Goomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;

	if (hasBeenSteppedOn || hasBeenAttacked)
	{
		left = right = top = bottom = 0;
	}
	else 	
		bottom = y + GOOMBA_BBOX_HEIGHT;
}

void Goomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	//Apply Gravity
	vy += 0.002 * dt;

	CGameObject::Update(dt, coObjects);
	HandleCollision(dt, coObjects);

	timeElapsed = GetTickCount64() - deadTime;

	if (state == GOOMBA_STATE_DIE && timeElapsed > GOOMBA_TIME_BEFORE_DISAPPEARED)
		isActive = false;
}

void Goomba::Render()
{
	if (!isActive)
		return;

	int ani = GOOMBA_ANI_WALKING;
	if (state == GOOMBA_STATE_DIE)
	{
		ani = GOOMBA_ANI_DIE;
	}
	else if (state == GOOMBA_STATE_DIE_ONESHOT)
		ani = GOOMBA_ANI_ONESHOT;

	animation_set->at(ani)->Render(nx, x, y);

	RenderBoundingBox();
}

void Goomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
			break;
		case GOOMBA_STATE_DIE:
			y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
			vx = 0;
			vy = 0;
			break;
		case GOOMBA_STATE_DIE_ONESHOT:
			vx = -GOOMBA_WALKING_SPEED + 0.04f;
			vy = -0.35;
			break;

	}
}

void Goomba::HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (hasBeenSteppedOn)
		return;

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

		if (nx != 0) vx *= -1; //Change direction when hit object
		
		if (ny != 0) vy = 0;

	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Goomba::OnSteppedOn()
{
	SetState(GOOMBA_STATE_DIE);
	deadTime = GetTickCount64();
	hasBeenSteppedOn = true;
}

void Goomba::OnAttacked()
{
	SetState(GOOMBA_STATE_DIE_ONESHOT);
	hasBeenAttacked = true;
}
