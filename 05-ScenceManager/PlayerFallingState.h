#pragma once
#include "PlayerState.h"

class PlayerFallingState : public PlayerState
{
	float fallTime;
public:
	PlayerFallingState();
	void Update(DWORD dt);
	void HandleKeyboard(DWORD dt);
	void UpdateAnimation();

	bool isSlowFalling;
};