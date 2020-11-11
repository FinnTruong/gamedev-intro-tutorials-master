#include "PlayerJumpingState.h"
#include "Player.h"
#include "PlayerIdleState.h"
#include "PlayerFallingState.h"
#include "PlayerFlyingState.h"

#define MARIO_LOW_JUMP_MULTIPLIER 0.1f

PlayerJumpingState::PlayerJumpingState()
{
	Mario->allow[MARIO_STATE_CROUCHING] = false;
	Mario->allow[MARIO_STATE_FLYING] = true;
	Mario->vy = -MARIO_JUMP_SPEED_Y;
	state = MARIO_STATE_JUMPING;

}

void PlayerJumpingState::Update(DWORD dt)
{
	if (Mario->vy == 0)
	{
		Mario->SetState(new PlayerIdleState());
		return;
	}
	 this->HandleKeyboard(dt);
}

void PlayerJumpingState::HandleKeyboard(DWORD dt)
{
	Mario->vx = 0;

	if (Mario->vy > 0)
		Mario->SetState(new PlayerFallingState());

	if (keyCode[DIK_LEFT])
	{		
		if (Mario->previousState == MARIO_STATE_RUNNING)
			Mario->vx = -MARIO_RUNNING_SPEED;
		else
			Mario->vx = -MARIO_WALKING_SPEED;
		Mario->nx = -1;
	}

	else if (keyCode[DIK_RIGHT])
	{
		if (Mario->previousState == MARIO_STATE_RUNNING)
			Mario->vx = MARIO_RUNNING_SPEED;
		else
			Mario->vx = MARIO_WALKING_SPEED;
		Mario->nx = 1;
	}

	if (Mario->vy < 0 && !keyCode[DIK_SPACE])
	{
		Mario->SetState(new PlayerFallingState());
		
	}

	/*if (Mario->abilityBar >= MARIO_FULL_ABILITY_BAR || Mario->isFlying)
	{
		if (Mario->level == MARIO_LEVEL_RACCOON)
			Mario->SetState(new PlayerFlyingState());
	}*/

}

void PlayerJumpingState::UpdateAnimation()
{
	switch (Mario->level)
	{
	case MARIO_LEVEL_SMALL:
		animation = MARIO_ANI_SMALL_JUMPING;
		break;
	case MARIO_LEVEL_BIG:
		animation = MARIO_ANI_BIG_JUMPING;
		if (keyCode[DIK_DOWN])
			animation = MARIO_ANI_BIG_CROUCHING;
		break;
	case MARIO_LEVEL_RACCOON:
		if (Mario->isFlying)
			animation = MARIO_ANI_RACCOON_FLYING;
		animation = MARIO_ANI_RACCOON_JUMPING;
		if (keyCode[DIK_DOWN])
			animation = MARIO_ANI_RACCOON_CROUCHING;
		break;
	case MARIO_LEVEL_FIRE:
		animation = MARIO_ANI_FIRE_JUMPING;
		if (keyCode[DIK_DOWN])
			animation = MARIO_ANI_FIRE_CROUCHING;
		break;
	}
}
