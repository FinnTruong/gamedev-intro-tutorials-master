#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "textures.h"

CTextureDatabase * CTextureDatabase::__instance = NULL;

CTextureDatabase::CTextureDatabase()
{

}

CTextureDatabase *CTextureDatabase::GetInstance()
{
	if (__instance == NULL) __instance = new CTextureDatabase();
	return __instance;
}

void CTextureDatabase::Add(int id, LPCWSTR filePath, D3DCOLOR transparentColor)
{
	textures[id] = CGame::GetInstance()->LoadTexture(filePath);
}

LPDIRECT3DTEXTURE9 CTextureDatabase::Get(unsigned int i) 
{
	return textures[i];
}



