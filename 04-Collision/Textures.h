#pragma once
#include <unordered_map>
#include <d3dx9.h>

using namespace std;

/*
	Manage texture database
*/
class CTextureDatabase
{
	static CTextureDatabase * __instance;

	unordered_map<int, LPDIRECT3DTEXTURE9> textures;

public: 
	CTextureDatabase();
	void Add(int id, LPCWSTR filePath, D3DCOLOR transparentColor);
	LPDIRECT3DTEXTURE9 Get(unsigned int i);

	static CTextureDatabase * GetInstance();
};