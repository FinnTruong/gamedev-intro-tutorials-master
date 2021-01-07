#pragma once

#include <vector>
#include "GameObject.h"


struct Cell
{
	vector<LPGAMEOBJECT> gameObjs;
};

class Grid
{
private:
	std::vector<Cell> m_cells;  //Treat 1D vector as 2D vector for cache friendliness
	int m_cellSize;
	int m_width;
	int m_height;
	int m_numXCells;
	int m_numYCells;

public:
	Grid(int width, int height, int cellSize);
	~Grid();
	
	void AddObj(LPGAMEOBJECT gameObj);
	//Gets Cell based on cell coordinate
	Cell* GetCell(int x, int y);
	//Gets Cell based on viewport coordinate
	Cell* GetCell(float x, float y);

};

