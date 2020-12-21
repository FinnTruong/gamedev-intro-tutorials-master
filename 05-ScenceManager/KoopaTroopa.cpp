#include "KoopaTroopa.h"
#include "Player.h"
#include "OneWayPlatform.h"
#include "Brick.h"
#include "QuestionBlock.h"

KoopaTroopa::KoopaTroopa(int _type)
{
	tag = Tag::KOOPA;
	type = _type;
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
	CalcPotentialOverlapped(coObjects);

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

		if (ny != 0)
			vy = 0;

		if (ny < 0)
			isGrounded = true;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];


			if (e->obj->tag != Tag::ONE_WAY_PLATFORM)
			{
				if (ny != 0) vy = 0;
			}

			if (e->obj->tag == Tag::ONE_WAY_PLATFORM)
			{

				//Collision from top
				if (e->ny != 0)
				{
					if (e->ny < 0)
					{
						vy = 0;
					}
					else
					{
						y += dy;
					}
				}
			}

			if (ny < 0)
			{
				//Expand Boundary by half object width  on each side if smaller than object width
				if (e->obj->boundingBox.right - e->obj->boundingBox.left <= KOOPA_BBOX_WIDTH)
					boundary = Vector2(e->obj->boundingBox.left + e->obj->x - this->boundingBox.left - KOOPA_BBOX_WIDTH/4.f,
						e->obj->boundingBox.right + e->obj->x - this->boundingBox.right + KOOPA_BBOX_WIDTH / 4.f);
				else
					boundary = Vector2(e->obj->boundingBox.left + e->obj->x - this->boundingBox.left,
						e->obj->boundingBox.right + e->obj->x - this->boundingBox.right);
			}

			if (state == KOOPA_STATE_WALKING && (x <= boundary.x || x >= boundary.y))
			{
				vx *= -1;
			}

			if (nx != 0)
			{
				if (state == KOOPA_STATE_SPIN)
				{
					if (e->obj->tag == Tag::BRICK)
					{
						auto brick = dynamic_cast<Brick*>(e->obj);
						brick->SetState(BRICK_STATE_SHATTER);
					}

					if (e->obj->tag == Tag::QUESTION_BLOCK)
					{
						auto questionBlock = dynamic_cast<QuestionBlock*>(e->obj);
						questionBlock->SetState(QUESTION_BLOCK_STATE_HIT_FROM_SIDE);
					}
				}

				
			}
		}

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

void KoopaTroopa::OnOverlapped(LPGAMEOBJECT obj)
{
	if (obj->tag == Tag::TAIL)
	{
		SetState(KOOPA_STATE_SHELL);		
		vy = -0.15f;
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