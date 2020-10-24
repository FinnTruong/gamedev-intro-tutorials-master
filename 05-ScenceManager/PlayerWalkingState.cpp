#include "PlayerWalkingState.h"
#include "Game.h"
#include "Player.h"
#include "PlayerIdleState.h"
#include "PlayerRunningState.h"

PlayerWalkingState::PlayerWalkingState()
{
	Mario->allow[MARIO_STATE_CROUCHING] = false;
	state = MARIO_STATE_WALKING;
	moveInput = 0;	
}

void PlayerWalkingState::Update(DWORD dt)
{
	Mario->abilityBar = Mario->abilityBar <= 0 ? 0 : Mario->abilityBar - 1 * dt;

	Mario->vx = MARIO_WALKING_SPEED * moveInput;

	this->HandleKeyboard(dt);
}

void PlayerWalkingState::HandleKeyboard(DWORD dt)
{
	if (keyCode[DIK_A] && !Mario->isAttacking)
	{
		Mario->SetState(new PlayerRunningState());
	}
	else if (keyCode[DIK_RIGHT])
	{
		if (isSkiding)
		{
			moveInput = -0.5;
			Mario->nx = -1;
			return;
		}
		moveInput = moveInput >= 1 ? 1 : moveInput + MARIO_ACCELERATION_MULTIPLIER * dt;
		Mario->nx = 1;
	}
	else if (keyCode[DIK_LEFT])
	{
		if (isSkiding)
		{
			moveInput = 0.5;
			Mario->nx = 1;
			return;
		}
		moveInput = moveInput <= -1 ? -1 : moveInput - MARIO_ACCELERATION_MULTIPLIER * dt;
		Mario->nx = -1;
	}
	else
	{		
		Mario->nx = Mario->facingDirection;
		Mario->SetState(new PlayerIdleState());
	}


}

void PlayerWalkingState::UpdateAnimation()
{
	int movingDirection =1;
	if (keyCode[DIK_RIGHT])
		movingDirection = 1;
	else if (keyCode[DIK_LEFT])
		movingDirection = -1;


	switch (Mario->level)
	{
	case MARIO_LEVEL_SMALL:
		if (GetTickCount64() - Mario->releaseMoveInputTime <= 150 && Mario->vx >= MARIO_WALKING_SPEED)
		{
			if (Mario->facingDirection != movingDirection)
			{
				isSkiding = true;
				animation = MARIO_ANI_SMALL_SKIDING;				
			}
			else
			{
				isSkiding = false;
				animation = MARIO_ANI_SMALL_WALKING;
			}
		}
		else
		{
			isSkiding = false;
			animation = MARIO_ANI_SMALL_WALKING;
		}
		break;
	case MARIO_LEVEL_BIG:
		if (GetTickCount64() - Mario->releaseMoveInputTime <= 150)
		{
			if (Mario->facingDirection != movingDirection)
			{
				isSkiding = true;
				animation = MARIO_ANI_BIG_SKIDING;
			}
			else
			{
				isSkiding = false;
				animation = MARIO_ANI_BIG_WALKING;
			}
		}
		else
		{
			isSkiding = false;
			animation = MARIO_ANI_BIG_WALKING;
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
				animation = MARIO_ANI_RACCOON_WALKING;
			}
		}
		else
		{
			isSkiding = false;
			animation = MARIO_ANI_RACCOON_WALKING;
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
				animation = MARIO_ANI_FIRE_WALKING;
			}
		}
		else
		{
			isSkiding = false;
			animation = MARIO_ANI_FIRE_WALKING;
		}
		break;
	}
}
