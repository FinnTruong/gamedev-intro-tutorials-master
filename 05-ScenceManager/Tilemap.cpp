#include "Tilemap.h"
#include "GameGlobal.h"
#include "Game.h"

Tilemap::Tilemap(int ID, LPCWSTR filePath_texture, LPCWSTR filePath_data, int num_row_on_texture, int num_col_on_textture, int num_row_on_tilemap, int num_col_on_tilemap, int tileset_width, int tileset_height)
{
	id = ID;

	this->filePath_texture = filePath_texture;
	this->filePath_data = filePath_data;

	this->num_row_on_texture = num_row_on_texture;
	this->num_col_on_textture = num_col_on_textture;
	this->num_row_on_tilemap = num_row_on_tilemap;
	this->num_col_on_tilemap = num_col_on_tilemap;
	this->tileset_width = tileset_width;
	this->tileset_height = tileset_height;

	LoadMap();
	Load();
}


void Tilemap::LoadMap()
{
	CTextureDatabase* texture = CTextureDatabase::GetInstance();

	texture->Add(id, filePath_texture, D3DCOLOR_XRGB(255, 0, 255));

	LPDIRECT3DTEXTURE9 texTileMap = texture->Get(id);


	int id_sprite = 0;
	for (int i = 0; i < num_row_on_texture; i++)
	{
		for (int j = 0; j < num_col_on_textture; j++)
		{
			int id_SPRITE = id + id_sprite;
			sprites->Add(id_SPRITE, tileset_width * j, tileset_height * i, tileset_width * (j + 1), tileset_height * (i + 1), texTileMap);
			id_sprite = id_sprite + 1;
		}
	}
	DebugOut(L"[INFO] Load map complete \n");
}

void Tilemap::Load()
{
	DebugOut(L"[INFO] Start loading map resources from : %s \n", filePath_data);

	ifstream fs(filePath_data, ios::in);

	if (fs.fail())
	{
		DebugOut(L"[ERROR] TileMap::Load_MapData failed: ID=%d", id);
		fs.close();
		return;
	}

	string line;

	while (!fs.eof())
	{
		getline(fs, line);
		// Lưu sprite tile vào vector tilemap
		vector<LPSPRITE> spriteline;
		stringstream ss(line);
		int n;

		while (ss >> n)
		{
			int idTile = id + n;
			spriteline.push_back(sprites->Get(idTile));
		}

		tilemap.push_back(spriteline);
	}

	fs.close();

	DebugOut(L"[INFO] Done loading map resources %s\n", filePath_data);
}


void Tilemap::Draw(int tilemap_x_offset,int tilemap_y_offset)
{
	int cam_x, cam_y;
	CGame::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition(cam_x, cam_y);

	int firstcol = (int)cam_x / tileset_width;
	int lastcol = firstcol + (SCREEN_WIDTH / tileset_width);
	/*lastcol = lastcol > 175 ? 175 : lastcol;*/


	for (int i = 0; i < num_row_on_tilemap; i++)
	{
		for (int j = firstcol; j < lastcol; j++)
		{
			float x = (float)(tileset_width * (j - firstcol) + cam_x - (int)cam_x % tileset_width + tilemap_x_offset);
			float y = (float)(tileset_height * i + tilemap_y_offset);
			
			if (j >= num_col_on_tilemap)
				continue;

			tilemap[i][j]->Draw(1, x, y);
		}
	}
}

int Tilemap::GetWidthTileMap()
{
	return num_col_on_tilemap * tileset_width;
}

int Tilemap::GetHeightTileMap()
{
	return num_row_on_tilemap * tileset_height;
}

Tilemap::~Tilemap()
{
}


