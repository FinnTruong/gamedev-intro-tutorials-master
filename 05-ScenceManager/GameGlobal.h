#pragma once


//Hold global variable

#include <d3dx9.h>
#include <d3d9.h>
#include <dinput.h>
#include <WinUser.h>
#include <unordered_map>


#define SCREEN_WIDTH 272
#define SCREEN_HEIGHT 272
//---------------PLAYER STATS-------------------
#define Mario Player::GetInstance()

typedef D3DXVECTOR2 Vector2;
typedef D3DXVECTOR3 Vector3;

typedef LPDIRECT3DTEXTURE9 Texture;

extern std::unordered_map<int, bool> keyCode;

enum class Tag
{
	PLAYER,
	GROUND,
	PIPE,
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




