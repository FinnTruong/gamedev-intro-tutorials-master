#pragma once
#include"Sprites.h"
#include"Textures.h"
#include"Utils.h"
#include <iostream>
#include <fstream>
#include<string>
#include <sstream>

#define TILEMAP_Y_OFFSET -162.0

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

	void Load();
	void LoadMap();
	void Draw(int tilemap_x_offset,int tilemap_y_offset);

	int GetWidthTileMap();
	int GetHeightTileMap();
};
