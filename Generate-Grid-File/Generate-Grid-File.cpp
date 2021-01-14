
#include "Utils.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_TILEMAP	7

#define MAX_SCENE_LINE 1024

#define CELL_WIDTH		136
#define CELL_HEIGHT		136

using namespace std;

int numXCells, numYCells;

void _ParseSection_TILEMAP(ofstream& outputFile, string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return; // skip invalid lines

	int num_row_on_tilemap = atoi(tokens[5].c_str());
	int num_col_on_tilemap = atoi(tokens[6].c_str());
	int tileset_width = atoi(tokens[7].c_str());
	int tileset_height = atoi(tokens[8].c_str());


	int tilemap_width = tileset_width * num_col_on_tilemap;
	int tilemap_height = tileset_height * num_row_on_tilemap;

	numXCells = tilemap_width / CELL_WIDTH;
	numYCells = tilemap_height / CELL_HEIGHT;


	outputFile << tilemap_width << "\t" << tilemap_height << endl;
}

void _ParseSection_OBJECTS(ofstream& outputFile, string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an object set must have at least id, x, y

	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	int cell_x = (int)(x / CELL_WIDTH);
	int cell_y = (int)(y / CELL_HEIGHT);

	if (cell_x < 0) cell_x = 0;
	else if (cell_x >= numXCells) cell_x = numXCells;

	if (cell_y < 0) cell_y = 0;
	else if (cell_y >= numYCells) cell_y = numYCells;

	string cellX = to_string(cell_x);
	string cellY = to_string(cell_y);

	outputFile << cellY << "\t" << cellX << "\t";

	for (auto token : tokens)
	{
		outputFile << token.c_str() << "\t";
	}

	outputFile << endl;
}

int main()
{
	string sceneFilePath = "world1-4.txt";
	ofstream outputFile("world1-4_grid.txt");
	outputFile.clear();

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[OBJECTS]")
		{
			section = SCENE_SECTION_OBJECTS;
			continue;
		}
		if (line == "[TILEMAP]")
		{
			section = SCENE_SECTION_TILEMAP;
			continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
			case SCENE_SECTION_TILEMAP:
				_ParseSection_TILEMAP(outputFile, line); break;
			case SCENE_SECTION_OBJECTS: 
				_ParseSection_OBJECTS(outputFile, line);
				break;
		}
	}

	f.close();

}

