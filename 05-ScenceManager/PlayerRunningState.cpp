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
	if (Mario->holdingObject)
	{
		Mario->abilityBar -= 0.5 * dt;
		if (Mario->abilityBar <= 0)
			Mario->abilityBar = 0;
	}
	else
		Mario->abilityBar += 0.5 * dt;

	if (Mario->abilityBar == MARIO_FULL_ABILITY_BAR)
		Mario->abilityBar = MARIO_FULL_ABILITY_BAR;

	Mario->HandleMovement(dt);

	HandleKeyboard(dt);
}

void PlayerRunningState::HandleKeyboard(DWORD dt)
{
	
	if (!keyCode[DIK_A] || Mario->isAttacking)
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
	switch (Mario->level)
	{
	case MARIO_LEVEL_SMALL:
		animation = MARIO_ANI_SMALL_RUNNING;
		break;
	case MARIO_LEVEL_BIG:
		animation = MARIO_ANI_BIG_RUNNING;
		break;
	case MARIO_LEVEL_RACCOON:
		animation = MARIO_ANI_RACCOON_RUNNING;
		break;
	case MARIO_LEVEL_FIRE:
		animation = MARIO_ANI_FIRE_RUNNING;
		break;
	}
}
