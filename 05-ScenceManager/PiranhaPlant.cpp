#include "PiranhaPlant.h"
#include "Player.h"

PiranhaPlant::PiranhaPlant(float posX, float posY)
{
	tag = Tag::PIRANHA_PLANT;
	x = posX;
	y = posY;
	start_x = posX;
	start_y = posY;
	isTrigger = true;
	plantHeight = PIRANHA_HEIGHT;
}


void PiranhaPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + PIRANHA_WIDTH;
	bottom = y + PIRANHA_HEIGHT;
}

void PiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	float delta = start_y - y;
	
	nx = x > Mario->x ? 1 : -1;

	//RISE
	if (IsPlayerInRange())
	{
		if (y == start_y && !isCoolingDown)
		{
			isMoving = true;
			vy = -PIRANHA_SPEED;
		}
	}
	//STOP AT TOP
	if (delta > plantHeight)
	{
		isMoving = false;
		vy = 0;
		y = start_y - plantHeight;
		attackTime = GetTickCount64();
	}
	//ATTACK
	if (delta == plantHeight && vy == 0)
	{
		canDamagePlayer = true;
		ULONGLONG dt = GetTickCount64() - attackTime;
		//DELAY BEFORE ATTACK
		if (dt > PIRANHA_ATTACK_TIME / 2 && !hasAttacked)
		{
			Attack();
		}
		//SINK AFTER ATTACK
		if (dt > PIRANHA_ATTACK_TIME)
		{
			canDamagePlayer = false;
			isMoving = true;
			hasAttacked = false;
			vy = PIRANHA_SPEED;
		}
	}
	//STOP AT BOTTOM
	if (y > start_y)
	{
		isMoving = false;		
		y = start_y;
		vy = 0;
		cooldownTime = GetTickCount64();
		isCoolingDown = true;
	}


	//CHECK COOLDOWN

	ULONGLONG cooldownTimer = GetTickCount64() - cooldownTime;
	if (cooldownTimer > PIRANHA_COOLDOWN_TIME && isCoolingDown)
	{		
		isCoolingDown = false;		
	}


	y += dy;

}

void PiranhaPlant::Render()
{
	if (!isActive)
		return;

	animation_set->at(PIRANHA_ANI_BITTING)->Render(-nx, x, y);

	RenderBoundingBox();
}


void PiranhaPlant::SetState(int state)
{
	switch (state)
	{
	case PIRANHA_STATE_RISE:
		break;
	case PIRANHA_STATE_ATTACK:
		break;
	case PIRANHA_STATE_SINK:
		break;
	default:
		break;
	}
}

void PiranhaPlant::Attack()
{
	hasAttacked = true;
}

bool PiranhaPlant::IsPlayerInRange()
{
	float distanceToPlayer = abs(x - Mario->x);
	return (distanceToPlayer >= PIRANHA_MIN_DISTANCE_TO_PLAYER && distanceToPlayer <= PIRANHA_MAX_DISTANCE_TO_PLAYER);
}