#pragma once

#include "PlayerState.h"


class PlayerIdleState : public PlayerState
{
public:
	PlayerIdleState();
	void Update(DWORD dt);
	void HandleKeyboard(DWORD dt);
	void UpdateAnimation();
};

