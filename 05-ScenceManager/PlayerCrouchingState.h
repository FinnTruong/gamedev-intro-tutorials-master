#pragma once
#include "PlayerState.h"
#include <d3dx9tex.h>

class PlayerCrouchingState: public PlayerState
{
public:
	PlayerCrouchingState();
	void Update(DWORD dt);
	void HandleKeyboard(DWORD dt);
	void UpdateAnimation();
};

