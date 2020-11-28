#include "PlayerAttackingState.h"
#include "Player.h"
#include "PlayerWalkingState.h"
#include "PlayerIdleState.h"
#include "Fireball.h"
#include "Utils.h"

PlayerAttackingState::PlayerAttackingState()
{
	state = MARIO_STATE_ATTACKING;
	Mario->isAttacking = true;
}

PlayerAttackingState::~PlayerAttackingState()
{

}

void PlayerAttackingState::Update(DWORD dt)
{
	if (!Mario->isAttacking)
	{
		if (keyCode[DIK_LEFT] || keyCode[DIK_RIGHT])
			Mario->SetState(new PlayerWalkingState());
		else
			Mario->SetState(new PlayerIdleState());
	}
}

void PlayerAttackingState::HandleKeyboard(DWORD dt)
{

}

void PlayerAttackingState::UpdateAnimation()
{
	switch (Mario->level)
	{
	case MARIO_LEVEL_RACCOON:		
		animation = MARIO_ANI_RACCOON_ATTACKING;
		break;
	case MARIO_LEVEL_FIRE:
		animation = MARIO_ANI_FIRE_ATTACKING;
		break;
	default:
		break;
	}
}

void PlayerAttackingState::SpinTail()
{

}

void PlayerAttackingState::ShootFireball()
{
	auto obj = new Fireball();
	DebugOut(L"Shoot");
	obj->Fire(2,Mario->x,Mario->y);
}
