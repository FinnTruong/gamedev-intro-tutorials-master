#include "Goomba.h"
#include "Ground.h"
#include "Utils.h"
#include "Player.h"

Goomba::Goomba()
{
	tag = Tag::GOOMBA;
	type = GoombaType::GOOMBA;
	SetLevel(GOOMBA_LEVEL_GOOMBA);
	SetState(GOOMBA_STATE_WALKING);

	//numOfLowJump = 0;
	//lowJumpMax = 3;
	//type = GoombaType::PARAGOOMBA;
	//SetLevel(GOOMBA_LEVEL_PARAGOOMBA);
	//SetState(PARAGOOMBA_STATE_WALKING);
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
	CGameObject::Update(dt, coObjects);
	HandleCollision(dt, coObjects);
	
	UpdateParagoomba();

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

	switch (state)
	{
	case PARAGOOMBA_STATE_WALKING:
		ani = PARAGOOMBA_ANI_WALKING;
		break;
	case PARAGOOMBA_STATE_ATTACKING:
		ani = highJump ? PARAGOOMBA_ANI_ATTACKING : PARAGOOMBA_ANI_WALKING;
		break;
	case GOOMBA_STATE_WALKING:
		ani = type == GoombaType::GOOMBA ? GOOMBA_ANI_WALKING : PARAGOOMBA_ANI_WALKING_NO_WING;
		break;
	case GOOMBA_STATE_DIE:
		ani = type == GoombaType::GOOMBA ? GOOMBA_ANI_DIE : PARAGOOMBA_ANI_DIE;
		break;
	case GOOMBA_STATE_DIE_ONESHOT:
		ani = type == GoombaType::GOOMBA ? GOOMBA_ANI_ONESHOT : PARAGOOMBA_ANI_ONESHOT;
	default:
		break;
	}
	
	if (level == GOOMBA_LEVEL_PARAGOOMBA)
	{
		animation_set->at(ani)->Render(nx, x - 2, y - 8);
	}
	else
		animation_set->at(ani)->Render(nx, x, y);

	RenderBoundingBox();
}

void Goomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case PARAGOOMBA_STATE_WALKING:
			break;
		case PARAGOOMBA_STATE_ATTACKING:
			break;
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
			vy = -0.35f;
			break;
	}
}

void Goomba::HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (hasBeenSteppedOn)
		return;

	//Apply Gravity
	vy += 0.0005f * dt;

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

		if (nx != 0) vx *= -1; //Change direction when hit object
		
		if (ny != 0) vy = 0;

		if (ny < 0)
			isGrounded = true;

	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Goomba::OnOverlapped(LPGAMEOBJECT obj)
{
	if (obj->tag == Tag::TAIL)
	{
		OnAttacked();
	}
}

void Goomba::OnSteppedOn()
{
	if (level == GOOMBA_LEVEL_PARAGOOMBA)
	{
		SetLevel(GOOMBA_LEVEL_GOOMBA);
		SetState(GOOMBA_STATE_WALKING);
	}
	else
	{
		SetState(GOOMBA_STATE_DIE);
		deadTime = GetTickCount64();
		hasBeenSteppedOn = true;
	}
}

void Goomba::OnAttacked()
{
	SetState(GOOMBA_STATE_DIE_ONESHOT);
	hasBeenAttacked = true;
}

void Goomba::UpdateParagoomba()
{
	if (level != GOOMBA_LEVEL_PARAGOOMBA)
		return;

	ULONGLONG timeSinceFinishAttack = GetTickCount64() - finishAttackTime;

	if (IsPlayerInRange() && state != PARAGOOMBA_STATE_ATTACKING && timeSinceFinishAttack > PARAGOOMBA_ATTACK_COOLDOWN)
	{
		SetState(PARAGOOMBA_STATE_ATTACKING);
	}

	if (state == PARAGOOMBA_STATE_WALKING && isGrounded && timeSinceFinishAttack > PARAGOOMBA_TURN_DELAY)
	{
		//float turnDelayTimer;
		vx = x >= Mario->x ? -GOOMBA_WALKING_SPEED : GOOMBA_WALKING_SPEED;
	}

	if (state == PARAGOOMBA_STATE_ATTACKING && isGrounded)
	{
		if (numOfLowJump > lowJumpMax)
		{
			numOfLowJump = 0;
			finishAttackTime = GetTickCount64();
			SetState(PARAGOOMBA_STATE_WALKING);
			return;
		}
		if (numOfLowJump == lowJumpMax)
		{
			vy = -PARAGOOMBA_HIGH_JUMP_SPEED;
			//isGrounded = false;
			numOfLowJump++;
			highJump = true;
		}
		else
		{
			vy = -PARAGOOMBA_LOW_JUMP_SPEED;
			//isGrounded = false;
			numOfLowJump++;
			highJump = false;
		}
	}
}

bool Goomba::IsPlayerInRange()
{
	return abs(x - Mario->x) <= MAX_DISTANCE_TO_PLAYER;
}

Goomba::~Goomba()
{

}
