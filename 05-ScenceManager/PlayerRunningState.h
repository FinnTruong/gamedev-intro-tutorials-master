#pragma once
#include "PlayerState.h"

class PlayerRunningState: public PlayerState
{
public:
	PlayerRunningState();
	void Update(DWORD dt);
	void HandleKeyboard(DWORD dt);
	void UpdateAnimation();

	int moveInput;
	bool isSkiding;
	float previousSpeed;
};

