#include "PlayerRunningMaxState.h"
#include "Player.h"
#include "PlayerIdleState.h"
#include "PlayerRunningState.h"
#include "PlayerWalkingState.h"

PlayerRunningMaxState::PlayerRunningMaxState()
{
	Mario->allow[MARIO_STATE_FLYING] = false;
	state = MARIO_STATE_RUNNING_MAX;
}

void PlayerRunningMaxState::Update(DWORD dt)
{
	Mario->HandleMovement(dt);
	HandleKeyboard(dt);
}

void PlayerRunningMaxState::HandleKeyboard(DWORD dt)
{
	if (keyCode[DIK_A])
	{

		if (keyCode[DIK_RIGHT])
		{
			Mario->nx = 1;
		}
		else if (keyCode[DIK_LEFT])
		{
			Mario->nx = -1;
		}
		else
		{
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

}

void PlayerRunningMaxState::UpdateAnimation()
{
	switch (Mario->level)
	{
	case MARIO_LEVEL_SMALL:
		animation = MARIO_ANI_SMALL_RUNNING_MAX;
		break;
	case MARIO_LEVEL_BIG:
		animation = MARIO_ANI_BIG_RUNNING_MAX;
		break;
	case MARIO_LEVEL_RACCOON:
		animation = MARIO_ANI_RACCOON_RUNNING_MAX;
		break;
	case MARIO_LEVEL_FIRE:
		animation = MARIO_ANI_FIRE_RUNNING_MAX;
		break;
	}
}