#pragma once
#include "PlayerState.h"

class PlayerJumpingState : public PlayerState
{
public:
	PlayerJumpingState(float jumpSpeed);
	void Update(DWORD dt);
	void HandleKeyboard(DWORD dt);
	void UpdateAnimation();

};

