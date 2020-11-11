#include "PlayerRunningState.h"
#include "PlayerIdleState.h"
#include "PlayerWalkingState.h"
#include "PlayerCrouchingState.h"
#include "PlayerRunningMaxState.h"
#include "Game.h"
#include "Player.h"

PlayerRunningState::PlayerRunningState()
{
	state = MARIO_STATE_RUNNING;
	moveInput = Mario->nx;
	previousSpeed = Mario->vx;
}

void PlayerRunningState::Update(DWORD dt)
{
	Mario->abilityBar += 1 * dt;

	if (Mario->abilityBar == MARIO_FULL_ABILITY_BAR)
		Mario->abilityBar = MARIO_FULL_ABILITY_BAR;

	Mario->vx = MARIO_RUNNING_SPEED * moveInput;

	this->HandleKeyboard(dt);
}

void PlayerRunningState::HandleKeyboard(DWORD dt)
{
	if (keyCode[DIK_A] && !Mario->isAttacking)
	{
		if (keyCode[DIK_RIGHT])
		{
			/*if (isSkiding)
			{
				moveInput = -0.5;
				Mario->nx = -1;
				return;
			}*/
			moveInput = moveInput >= 1 ? 1 : moveInput + MARIO_ACCELERATION_MULTIPLIER * dt;
			Mario->nx = 1;
		}
		else if (keyCode[DIK_LEFT])
		{
			/*if (isSkiding)
			{
				moveInput = 0.5;
				Mario->nx = 1;
				return;
			}*/
			moveInput = moveInput <= -1 ? -1 : moveInput - MARIO_ACCELERATION_MULTIPLIER * dt;
			Mario->nx = -1;
		}
		else
		{
			Mario->facingDirection = Mario->nx;
			Mario->SetState(new PlayerIdleState());
		}
	}
	else
	{
		if (keyCode[DIK_RIGHT] || keyCode[DIK_LEFT])
			Mario->SetState(new PlayerWalkingState());
		else
			Mario->SetState(new PlayerIdleState());
	}
	
	if (Mario->abilityBar >= MARIO_FULL_ABILITY_BAR)
	{
		Mario->SetState(new PlayerRunningMaxState());
	}
}

void PlayerRunningState::UpdateAnimation()
{
	int movingDirection = 1;
	if (keyCode[DIK_RIGHT])
		movingDirection = 1;
	else if (keyCode[DIK_LEFT])
		movingDirection = -1;


	switch (Mario->level)
	{
	case MARIO_LEVEL_SMALL:
		if (Mario->vx == 0)
			return;

		if (GetTickCount64() - Mario->releaseMoveInputTime <= 150)
		{
			if (Mario->facingDirection != movingDirection)
			{
				isSkiding = true;
				animation = MARIO_ANI_SMALL_SKIDING;
			}
			else
			{
				isSkiding = false;
				animation = MARIO_ANI_SMALL_RUNNING;
			}
		}
		else
		{
			isSkiding = false;
			animation = MARIO_ANI_SMALL_RUNNING;
		}
		break;
	case MARIO_LEVEL_BIG:
		if (GetTickCount64() - Mario->releaseMoveInputTime <= 150)
		{
			if (Mario->facingDirection != movingDirection )
			{
				isSkiding = true;
				animation = MARIO_ANI_BIG_SKIDING;
			}
			else
			{
				isSkiding = false;
				animation = MARIO_ANI_BIG_RUNNING;
			}
		}
		else
		{
			isSkiding = false;
			animation = MARIO_ANI_BIG_RUNNING;
		}
		break;
	case MARIO_LEVEL_RACCOON:
		if (GetTickCount64() - Mario->releaseMoveInputTime <= 150)
		{
			if (Mario->facingDirection != movingDirection)
			{
				isSkiding = true;
				animation = MARIO_ANI_RACCOON_SKIDING;
			}
			else
			{
				isSkiding = false;
				animation = MARIO_ANI_RACCOON_RUNNING;
			}
		}
		else
		{
			isSkiding = false;
			animation = MARIO_ANI_RACCOON_RUNNING;
		}
		break;
	case MARIO_LEVEL_FIRE:
		if (GetTickCount64() - Mario->releaseMoveInputTime <= 150)
		{
			if (Mario->facingDirection != movingDirection)
			{
				isSkiding = true;
				animation = MARIO_ANI_FIRE_SKIDING;
			}
			else
			{
				isSkiding = false;
				animation = MARIO_ANI_FIRE_RUNNING;
			}
		}
		else
		{
			isSkiding = false;
			animation = MARIO_ANI_FIRE_RUNNING;
		}
		break;
	default:
		break;
	}
}
