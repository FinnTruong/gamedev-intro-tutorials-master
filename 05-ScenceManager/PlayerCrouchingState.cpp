#include "PlayerCrouchingState.h"
#include "Player.h"
#include "PlayerIdleState.h"


PlayerCrouchingState::PlayerCrouchingState()
{
	Mario->SetPosition(Mario->x, Mario->y + (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT));
	state = MARIO_STATE_CROUCHING;
}

void PlayerCrouchingState::Update(DWORD dt)
{
	if (Mario->vx > 0)
	{
		Mario->vx -= 0.00015f * dt;
		if (Mario->vx < 0)
			Mario->vx = 0;
	}
	else if (Mario->vx < 0) {
		Mario->vx += 0.00015f * dt;
		if (Mario->vx > 0)
			Mario->vx = 0;
	}
	this->HandleKeyboard(dt);
}

void PlayerCrouchingState::HandleKeyboard(DWORD dt)
{
	if (!keyCode[DIK_DOWN])
	{
		//if (Mario->state != MARIO_STATE_CROUCHING)
			Mario->SetPosition(Mario->x, Mario->y - (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT));
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
	case MARIO_LEVEL_FIRE:
		animation = MARIO_ANI_FIRE_CROUCHING;
		break;
	default:
		break;
	}
}
