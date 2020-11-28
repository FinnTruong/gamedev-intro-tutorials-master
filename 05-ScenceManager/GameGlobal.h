#pragma once


//Hold global variable

#include <d3dx9.h>
#include <d3d9.h>
#include <dinput.h>
#include <WinUser.h>
#include <unordered_map>

#define SCREEN_WIDTH 280
#define SCREEN_HEIGHT 260
//---------------PLAYER STATS-------------------
#define Mario Player::GetInstance()

extern std::unordered_map<int, bool> keyCode;

enum class Tag
{
	PLAYER,
	GROUND,
	ONEWAYPLATFORM,
	QUESTIONMARKBRICK,
	GOOMBA,
	KOOPA,
	ITEM,
	MUSHROOM,
	LEAF,
	FLOWER,
	TAIL
};



