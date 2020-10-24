#include "PlayerAttackingState.h"
#include "Player.h"
#include "PlayerWalkingState.h"
#include "PlayerIdleState.h"

PlayerAttackingState::PlayerAttackingState()
{
	state = MARIO_STATE_ATTACKING;
	Mario->isAttacking = true;
}

void PlayerAttackingState::Update(DWORD dt)
{
	this->HandleKeyboard(dt);
}

void PlayerAttackingState::HandleKeyboard(DWORD dt)
{
	if (!Mario->isAttacking)
	{
		Mario->SetState(new PlayerIdleState());
	}
}

void PlayerAttackingState::UpdateAnimation()
{
	switch (Mario->level)
	{
	case MARIO_LEVEL_RACCOON:
		animation = MARIO_ANI_RACCOON_ATTACKING;
		if (Mario->animation_set->at(animation)->GetCurrentFrame() == 2)
			Mario->isAttacking = false; 
		break;
	case MARIO_LEVEL_FIRE:
		break;
	default:
		break;
	}
}
