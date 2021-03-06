﻿#pragma once


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
	NONE,
	PLAYER,
	GROUND,
	PIPE,
	ONE_WAY_PLATFORM,
	MOVING_PLATFORM,
	QUESTION_BLOCK,
	P_BLOCK,
	BRICK,
	GOOMBA,
	KOOPA,
	PIRANHA_PLANT,
	VENUS_FIRE_TRAP,
	VENUS_FIREBALL,
	BOOMERANG,
	SLEDGE_BRO,
	ITEM,
	COIN,
	MUSHROOM,
	ONE_UP_MUSHROOM,
	LEAF,
	FLOWER,
	P_SWITCH,
	GOAL,
	TAIL
};

struct Rect
{
	float x = 0.f;
	float y = 0.f;
	float width = 0.f;
	float height = 0.f;

	Rect() {}

	Rect(float x, float y, float width, float height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	bool IsContain(Rect r)
	{
		return !((x + width < r.x) || (x > r.x + r.width) || (y < r.y - r.height) || (y - height > r.y));
	}
};





