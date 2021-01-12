#include "PlayerFlyingState.h"
#include "PlayerFallingState.h"
#include "Game.h"
#include "Player.h"

#define MARIO_FLYING_SPEED 0.08f

PlayerFlyingState::PlayerFlyingState()
{
	Mario->allow[MARIO_STATE_JUMPING] = true;
	Mario->vy = -0.15f;
	state = MARIO_STATE_FLYING;
	Mario->isFlying = true;
}

void PlayerFlyingState::Update(DWORD dt)
{	
	if (Mario->isFlying)
		Mario->isFlying = GetTickCount64() - Mario->releaseJumpInputTime > 1000 ? false : true;
	this->HandleKeyboard(dt);
}

void PlayerFlyingState::HandleKeyboard(DWORD dt)
{
	if (!Mario->isFlying)
	{
		Mario->SetState(new PlayerFallingState());
		Mario->abilityBar = 0;
		return;
	}

	if (keyCode[DIK_LEFT])
	{
		Mario->vx = -MARIO_FLYING_SPEED;
		Mario->nx = -1;
	}
	else if (keyCode[DIK_RIGHT])
	{
		Mario->vx = MARIO_FLYING_SPEED;
		Mario->nx = 1;
	}
}

void PlayerFlyingState::UpdateAnimation()
{
	switch (Mario->level)
	{
	case MARIO_LEVEL_RACCOON:
		animation = MARIO_ANI_RACCOON_FLYING;
		break;
	default:
		break;
	}
}
