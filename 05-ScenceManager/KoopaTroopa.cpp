#include "KoopaTroopa.h"
#include "Player.h"

KoopaTroopa::KoopaTroopa()
{
	tag = Tag::KOOPA;
	type = KoopaTroopaType::KOOPA_RED;
	SetState(KOOPA_STATE_WALKING);
}


void KoopaTroopa::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
	if (isBeingHeld)
	{
		//vx = Player::GetInstance()->vx;
		return;
	}

	nx = vx >= 0 ? 1 : -1;

	if (state == KOOPA_STATE_JUMPING && isGrounded)
	{
		vy = -KOOPA_JUMPING_SPEED;
	}

	HandleCollision(dt, coObjects);
}

void KoopaTroopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
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

void KoopaTroopa::HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += 0.0005f * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		isGrounded = false;
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

		if (nx != 0)
		{
			vx *= -1; //Change direction when hit object
		}

		if (ny != 0) vy = 0;

		if (ny < 0)
			isGrounded = true;

	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void KoopaTroopa::Render()
{
	int ani = KOOPA_RED_ANI_WALKING;

	switch (state)
	{
	case KOOPA_STATE_JUMPING:
		ani = type == KoopaTroopaType::KOOPA_RED ? KOOPA_RED_ANI_JUMPING : KOOPA_GREEN_ANI_JUMPING;
		break;
	case KOOPA_STATE_WALKING:
		ani = type == KoopaTroopaType::KOOPA_RED ? KOOPA_RED_ANI_WALKING : KOOPA_GREEN_ANI_WALKING;
		break;
	case KOOPA_STATE_SHELL:
		ani = type == KoopaTroopaType::KOOPA_RED ? KOOPA_RED_ANI_SHELL : KOOPA_GREEN_ANI_SHELL;
		break;
	case KOOPA_STATE_SPIN:
		ani = type == KoopaTroopaType::KOOPA_RED ? KOOPA_RED_ANI_SPIN : KOOPA_GREEN_ANI_SPIN;
		break;
	case KOOPA_STATE_DIE:
		ani = type == KoopaTroopaType::KOOPA_RED ? KOOPA_RED_ANI_DIE : KOOPA_GREEN_ANI_DIE;
		break;
	default:
		break;
	}

	animation_set->at(ani)->Render(nx, x, y);

	RenderBoundingBox();
}

void KoopaTroopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPA_STATE_JUMPING:
		break;
	case KOOPA_STATE_WALKING:
		vx = KOOPA_WALKING_SPEED;
		break;
	case KOOPA_STATE_SHELL:
		y += (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_DIE) / 2 - 5;
		vx = 0;
		vy = 0;
		break;

	case KOOPA_STATE_SPIN:
		vx = -nx * 0.1f;
		break;
	case KOOPA_STATE_DIE:
		vx = -KOOPA_WALKING_SPEED + 0.04f;
		vy = -0.35f;
		break;
	}

}

void KoopaTroopa::OnSteppedOn()
{
	switch (state)
	{
	case KOOPA_STATE_JUMPING:
		SetState(KOOPA_STATE_WALKING);
		break;
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

void KoopaTroopa::OnAttacked()
{
	SetState(KOOPA_STATE_DIE);
}

KoopaTroopa::~KoopaTroopa()
{

}