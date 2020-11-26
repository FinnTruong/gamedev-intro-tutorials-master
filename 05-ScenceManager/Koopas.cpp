#include "Koopas.h"
#include "Player.h"

Koopa::Koopa()
{
	tag = Tag::KOOPA;
	SetState(KOOPA_STATE_WALKING);
}


void Koopa::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
	if (isBeingHeld)
	{
		//vx = Player::GetInstance()->vx;
		return;
	}
	vy += 0.002 * dt;
	HandleCollision(dt, coObjects);
}

void Koopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{	
	left = x;
	top = y;
	right = x + KOOPA_BBOX_WIDTH;
	
	if (isBeingHeld)
		left = right = top = bottom = 0;

	if (state == KOOPA_STATE_SHELL || state == KOOPA_STATE_SPIN)
	{
		bottom = y + KOOPA_BBOX_HEIGHT_DIE;
	}
	else if (state == KOOPA_STATE_DIE || isBeingHeld )
		left = right = top = bottom = 0;
	else
		bottom = y + KOOPA_BBOX_HEIGHT;
	

}

void Koopa::HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

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

void Koopa::Render()
{
	int ani = KOOPA_ANI_WALKING;
	if (state == KOOPA_STATE_SHELL) {
		ani = KOOPA_ANI_SHELL;
	}
	else if (state == KOOPA_STATE_SPIN) {
		ani = KOOPA_ANI_SPIN;
	}
	else if (state == KOOPA_STATE_DIE)
		ani = KOOPA_ANI_DIE;

	nx = vx >= 0 ? 1 : -1;

	animation_set->at(ani)->Render(nx, x, y);

	RenderBoundingBox();
}

void Koopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPA_STATE_WALKING:
		vx = KOOPA_WALKING_SPEED;
		break;
	case KOOPA_STATE_SHELL:
		y += (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_DIE) / 2 - 5;
		vx = 0;
		vy = 0;
		break;

	case KOOPA_STATE_SPIN:
		vx = -nx * 0.1;
		break;
	case KOOPA_STATE_DIE:
		vx = -KOOPA_WALKING_SPEED + 0.04f;
		vy = -0.35f;
		break;
	}

}

void Koopa::OnSteppedOn()
{
	switch (state)
	{
	case KOOPA_STATE_WALKING:
		SetState(KOOPA_STATE_SHELL);
		break;
	case KOOPA_STATE_SHELL:
		SetState(KOOPA_STATE_SPIN);
		break;
	case KOOPA_STATE_SPIN:
		SetState(KOOPA_STATE_SHELL);
		break;
	default:
		break;
	}
}

void Koopa::OnAttacked()
{
	SetState(KOOPA_STATE_DIE);
}