#pragma once
#include "PlayerState.h"
class PlayerRunningMaxState : public PlayerState
{
public:
	PlayerRunningMaxState();
	void Update(DWORD dt);
	void HandleKeyboard(DWORD dt);
	void UpdateAnimation();
};

