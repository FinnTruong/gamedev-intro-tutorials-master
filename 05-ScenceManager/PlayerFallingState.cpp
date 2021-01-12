#include "PlayerFallingState.h"
#include "Player.h"
#include "PlayerIdleState.h"

PlayerFallingState::PlayerFallingState()
{
	Mario->allow[MARIO_STATE_CROUCHING] = false;
	Mario->allow[MARIO_STATE_JUMPING] = false;
	Mario->allow[MARIO_STATE_FLYING] = false;
	state = MARIO_STATE_FALLING;
	Mario->isFlying = false;
	Mario->isGrounded = false;
	fallTime = GetTickCount64();
}

void PlayerFallingState::Update(DWORD dt)
{
	if (Mario->vy < 0)
	{
		Mario->vy += 0.0005f * dt;
	}

	if (Mario->vy == 0)
	{
		Mario->SetState(new PlayerIdleState());
		return;
	}
	this->HandleKeyboard(dt);
}

void PlayerFallingState::HandleKeyboard(DWORD dt)
{
	if (GetTickCount64() - Mario->releaseJumpInputTime <= 200.0f && Mario->level == MARIO_LEVEL_RACCOON)
	{
		Mario->isSlowFalling = true;
	}
	else
	{
		Mario->isSlowFalling = false;
	}
	/*Mario->vx = 0;

	

	if (keyCode[DIK_LEFT])
	{
		Mario->vx = Mario->previousState == MARIO_STATE_RUNNING ? -MARIO_RUNNING_SPEED : -MARIO_WALKING_SPEED;
		Mario->nx = -1;
	}

	else if (keyCode[DIK_RIGHT])
	{
		Mario->vx = Mario->previousState == MARIO_STATE_RUNNING ? MARIO_RUNNING_SPEED : MARIO_WALKING_SPEED;
		Mario->nx = 1;
	}*/
	Mario->HandleMovement(dt);
}

void PlayerFallingState::UpdateAnimation()
{
	switch (Mario->level)
	{
	case MARIO_LEVEL_SMALL:
		animation = Mario->abilityBar >= MARIO_FULL_ABILITY_BAR ? MARIO_ANI_SMALL_MAX_JUMPING : MARIO_ANI_SMALL_JUMPING;
		break;
	case MARIO_LEVEL_BIG:
		if (Mario->abilityBar >= MARIO_FULL_ABILITY_BAR)
		{
			animation = MARIO_ANI_BIG_MAX_JUMPING;
		}
		else
			animation = MARIO_ANI_BIG_FALLING;

		if (keyCode[DIK_DOWN])
			animation = MARIO_ANI_BIG_CROUCHING;
		break;
	case MARIO_LEVEL_RACCOON:
		if (Mario->abilityBar >= MARIO_FULL_ABILITY_BAR)
		{
			animation = MARIO_ANI_RACCOON_MAX_JUMPING;
		}
		else
			animation = Mario->isSlowFalling ? MARIO_ANI_RACCOON_SLOW_FALLING : MARIO_ANI_RACCOON_FALLING;

		if (keyCode[DIK_DOWN])
			animation = MARIO_ANI_RACCOON_CROUCHING;
		break;
	case MARIO_LEVEL_FIRE:
		if (Mario->abilityBar >= MARIO_FULL_ABILITY_BAR)
		{
			animation = MARIO_ANI_FIRE_MAX_JUMPING;
		}
		else
			animation = MARIO_ANI_FIRE_FALLING;

		if (keyCode[DIK_DOWN])
			animation = MARIO_ANI_FIRE_CROUCHING;
		break;
	default:
		break;
	}
}
