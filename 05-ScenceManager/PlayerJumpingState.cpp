#include "PlayerJumpingState.h"
#include "Player.h"
#include "PlayerIdleState.h"
#include "PlayerFallingState.h"
#include "PlayerFlyingState.h"

#define MARIO_LOW_JUMP_MULTIPLIER 0.1f

PlayerJumpingState::PlayerJumpingState(float jumpSpeed)
{
	Mario->allow[MARIO_STATE_CROUCHING] = false;
	Mario->allow[MARIO_STATE_FLYING] = true;
	
	Mario->vy = -jumpSpeed;
	Mario->isGrounded = false;
	state = MARIO_STATE_JUMPING;

}

void PlayerJumpingState::Update(DWORD dt)
{
	if (Mario->vy == 0 && Mario->isGrounded)
	{
		Mario->SetState(new PlayerIdleState());
		return;
	}
	 this->HandleKeyboard(dt);
}

void PlayerJumpingState::HandleKeyboard(DWORD dt)
{
	if (Mario->vy > 0)
		Mario->SetState(new PlayerFallingState());

	/*if (keyCode[DIK_LEFT])
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
	}*/

	Mario->HandleMovement(dt);

	if (Mario->vy < 0 && !keyCode[DIK_SPACE])
	{
		Mario->SetState(new PlayerFallingState());
		
	}

}

void PlayerJumpingState::UpdateAnimation()
{

	switch (Mario->level)
	{
	case MARIO_LEVEL_SMALL:
		animation = Mario->abilityBar >= MARIO_FULL_ABILITY_BAR ? MARIO_ANI_SMALL_MAX_JUMPING : MARIO_ANI_SMALL_JUMPING;
		break;
	case MARIO_LEVEL_BIG:
		animation = Mario->abilityBar >= MARIO_FULL_ABILITY_BAR ? MARIO_ANI_BIG_MAX_JUMPING : MARIO_ANI_BIG_JUMPING;
		if (keyCode[DIK_DOWN])
			animation = MARIO_ANI_BIG_CROUCHING;
		break;
	case MARIO_LEVEL_RACCOON:
		if (Mario->isFlying)
			animation = MARIO_ANI_RACCOON_FLYING;
		animation = Mario->abilityBar >= MARIO_FULL_ABILITY_BAR ? MARIO_ANI_RACCOON_MAX_JUMPING : MARIO_ANI_RACCOON_JUMPING;
		if (keyCode[DIK_DOWN])
			animation = MARIO_ANI_RACCOON_CROUCHING;
		break;
	case MARIO_LEVEL_FIRE:
		animation = Mario->abilityBar >= MARIO_FULL_ABILITY_BAR ? MARIO_ANI_FIRE_MAX_JUMPING : MARIO_ANI_FIRE_JUMPING;
		if (keyCode[DIK_DOWN])
			animation = MARIO_ANI_FIRE_CROUCHING;
		break;
	}
}
