#include "PlayerWalkingState.h"
#include "Game.h"
#include "Player.h"
#include "PlayerIdleState.h"
#include "PlayerRunningState.h"

PlayerWalkingState::PlayerWalkingState()
{
	Mario->allow[MARIO_STATE_CROUCHING] = false;
	Mario->isAttacking = false;
	state = MARIO_STATE_WALKING;
	moveInput = 0;	
}

void PlayerWalkingState::Update(DWORD dt)
{
	Mario->abilityBar = Mario->abilityBar <= 0 ? 0 : Mario->abilityBar - 1 * dt;

	Mario->HandleMovement(dt);
	
	HandleKeyboard(dt);
}

void PlayerWalkingState::HandleKeyboard(DWORD dt)
{
	if (keyCode[DIK_A] && !Mario->isAttacking)
	{
		Mario->SetState(new PlayerRunningState());
	}	
	else
	{		
		//Mario->nx = Mario->facingDirection;
		Mario->SetState(new PlayerIdleState());
	}


}

void PlayerWalkingState::UpdateAnimation()
{
	switch (Mario->level)
	{
	case MARIO_LEVEL_SMALL:
		animation = MARIO_ANI_SMALL_WALKING;
		break;
	case MARIO_LEVEL_BIG:
		animation = MARIO_ANI_BIG_WALKING;
		break;
	case MARIO_LEVEL_RACCOON:
		animation = MARIO_ANI_RACCOON_WALKING;
		break;
	case MARIO_LEVEL_FIRE:
		animation = MARIO_ANI_FIRE_WALKING;
		break;
	}
}
