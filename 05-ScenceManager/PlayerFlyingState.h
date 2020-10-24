#pragma once

#include "PlayerState.h"


class PlayerFlyingState : public PlayerState
{
public:
	PlayerFlyingState();
	void Update(DWORD dt);
	void HandleKeyboard(DWORD dt);
	void UpdateAnimation();
};