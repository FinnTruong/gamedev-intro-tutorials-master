#pragma once
#include"Sprites.h"
#include"Textures.h"
#include"Utils.h"
#include <iostream>
#include <fstream>
#include<string>
#include <sstream>

class Tilemap
{
	CSpriteDatabase* sprites = CSpriteDatabase::GetInstance();
	LPCWSTR filePath_data;
	LPCWSTR filePath_texture;

	int id;
	int num_row_on_tilemap, num_col_on_tilemap;
	int num_row_on_texture, num_col_on_textture;
	int tileset_width, tileset_height;

	vector<vector<LPSPRITE>> tilemap;

public:
	Tilemap(int ID, LPCWSTR filePath_texture, LPCWSTR filePath_data, int num_row_on_texture, int num_col_on_textture, int num_row_on_tilemap, int num_col_on_tilemap, int tileset_width = 32, int tileset_height = 32);
	~Tilemap();
	int GetWidthTileMap();
	void Load();
	void LoadMap();
	void Draw();
};
