#include "PlayerIdleState.h"
#include "PlayerWalkingState.h"
#include "PlayerCrouchingState.h"
#include "Game.h"
#include "Player.h"
PlayerIdleState::PlayerIdleState()
{
	Mario->allow[MARIO_STATE_JUMPING] = true;
	Mario->allow[MARIO_STATE_CROUCHING] = true;
	Mario->isAttacking = false;
	state = MARIO_STATE_IDLE;	

}

void PlayerIdleState::Update(DWORD dt)
{
	if (Mario->vx > 0) 
	{
		Mario->vx -= MARIO_DRAG_FORCE * dt;
		if (Mario->vx < 0)
			Mario->vx = 0;
	}
	else if (Mario->vx < 0) {
		Mario->vx += MARIO_DRAG_FORCE * dt;
		if (Mario->vx > 0)
			Mario->vx = 0;
	}
	int velDir = 0;
	if (keyCode[DIK_DOWN])
	{
		if (Mario->level != MARIO_LEVEL_SMALL)
			Mario->SetState(new PlayerCrouchingState());
		return;
	}

	if (keyCode[DIK_RIGHT])
	{
		velDir = 1;
	}
	else if (keyCode[DIK_LEFT])
	{
		velDir = -1;
	}


	Mario->isSkiding = velDir * Mario->facingDirection < 0;
	Mario->abilityBar = Mario->abilityBar <= 0 ? 0 : Mario->abilityBar - 1 * dt;

	this->HandleKeyboard(dt);
}

void PlayerIdleState::HandleKeyboard(DWORD dt)
{

	if (keyCode[DIK_LEFT] ||keyCode[DIK_RIGHT])
	{
		Mario->SetState(new PlayerWalkingState());
	}
	else if (keyCode[DIK_DOWN] && Mario->level == MARIO_LEVEL_BIG && Mario->allow[MARIO_STATE_CROUCHING])
	{
		Mario->SetState(new PlayerCrouchingState());
	}
}

void PlayerIdleState::UpdateAnimation()
{
	switch (Mario->level)
	{
	case MARIO_LEVEL_SMALL:
		animation = Mario->vx != 0 ? MARIO_ANI_SMALL_WALKING : MARIO_ANI_SMALL_IDLE;
		if (Mario->holdingObject)
			animation = animation = Mario->vx == 0 ? MARIO_ANI_SMALL_IDLE_HOLD : MARIO_ANI_SMALL_MOVING_HOLD;
		break;
	case MARIO_LEVEL_BIG:
		animation = Mario->vx != 0 ? MARIO_ANI_BIG_WALKING : MARIO_ANI_BIG_IDLE;
		if (Mario->holdingObject)
			animation = animation = Mario->vx == 0 ? MARIO_ANI_BIG_IDLE_HOLD : MARIO_ANI_BIG_MOVING_HOLD;
		break;
	case MARIO_LEVEL_RACCOON:
		animation = Mario->vx != 0 ? MARIO_ANI_RACCOON_WALKING : MARIO_ANI_RACCOON_IDLE;
		if (Mario->holdingObject)
			animation = animation = Mario->vx == 0 ? MARIO_ANI_RACCOON_IDLE_HOLD : MARIO_ANI_RACCOON_MOVING_HOLD;
		break;
	case MARIO_LEVEL_FIRE:
		animation = Mario->vx != 0 ? MARIO_ANI_FIRE_WALKING : MARIO_ANI_FIRE_IDLE;
		if (Mario->holdingObject)
			animation = animation = Mario->vx == 0 ? MARIO_ANI_FIRE_IDLE_HOLD : MARIO_ANI_FIRE_MOVING_HOLD;
		break;
	default:
		break;
	}
}
