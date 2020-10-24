#include "PlayerCrouchingState.h"
#include "Player.h"
#include "PlayerIdleState.h"


PlayerCrouchingState::PlayerCrouchingState()
{
	state = MARIO_STATE_CROUCHING;	
}

void PlayerCrouchingState::Update(DWORD dt)
{
	if (Mario->vx > 0)
	{
		Mario->vx -= 0.00015 * dt;
		if (Mario->vx < 0)
			Mario->vx = 0;
	}
	else if (Mario->vx < 0) {
		Mario->vx += 0.00015 * dt;
		if (Mario->vx > 0)
			Mario->vx = 0;
	}
	this->HandleKeyboard(dt);
}

void PlayerCrouchingState::HandleKeyboard(DWORD dt)
{
	if (!keyCode[DIK_DOWN])
	{
		Mario->SetState(new PlayerIdleState());
	}
}

void PlayerCrouchingState::UpdateAnimation()
{
	switch (Mario->level)
	{
	case MARIO_LEVEL_BIG:
		animation = MARIO_ANI_BIG_CROUCHING;
		break;
	case MARIO_LEVEL_RACCOON:
		animation = MARIO_ANI_RACCOON_CROUCHING;
		break;
	default:
		break;
	}
}
