#include "Sprites.h"
#include "Game.h"
#include "Utils.h"

CSprite::CSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;
}

CSpriteDatabase * CSpriteDatabase::__instance = NULL;

CSpriteDatabase *CSpriteDatabase::GetInstance()
{
	if (__instance == NULL) __instance = new CSpriteDatabase();
	return __instance;
}

void CSprite::Draw(int direction,float x, float y, int alpha)
{
	CGame * game = CGame::GetInstance();

	float width = right - left;
	float height = bottom - top;

   	game->Draw(direction, x, y, texture, left, top, right, bottom, alpha);
}

void CSpriteDatabase::Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	LPSPRITE s = new CSprite(id, left, top, right, bottom, tex);
	sprites[id] = s;

	DebugOut(L"[INFO] sprite added: %d, %d, %d, %d, %d \n", id, left, top, right, bottom);
}


LPSPRITE CSpriteDatabase::Get(int id)
{
	return sprites[id];
}

/*
	Clear all loaded textures
*/
void CSpriteDatabase::Clear()
{
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}

	sprites.clear();
}



