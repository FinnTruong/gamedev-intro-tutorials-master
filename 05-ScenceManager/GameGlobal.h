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
	ONE_WAY_PLATFORM,
	QUESTION_MARK_BRICK,
	GOOMBA,
	KOOPA,
	PIRANHA_PLANT,
	VENUS_FIRE_TRAP,
	VENUS_FIREBALL,
	ITEM,
	MUSHROOM,
	LEAF,
	FLOWER,
	TAIL
};




