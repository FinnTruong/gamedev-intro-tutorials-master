#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <vector>

class PlayerState
{
public:
	PlayerState() {}
	virtual void Update(DWORD dt) = 0;
	virtual void HandleKeyboard(DWORD dt) = 0;
	virtual void UpdateAnimation() = 0;
	int state = 0;
	int animation = 0;
};

