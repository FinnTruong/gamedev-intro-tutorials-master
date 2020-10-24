#pragma once
#include "PlayerState.h"

class PlayerWalkingState : public PlayerState
{
public:
	PlayerWalkingState();

	float moveInput;
	bool isSkiding;
	

	void Update(DWORD dt);
	void HandleKeyboard(DWORD dt);
	void UpdateAnimation();
};

