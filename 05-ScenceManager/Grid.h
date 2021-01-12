#pragma once

#include <vector>
#include "GameObject.h"
#include "Cell.h"

class Grid
{
	Rect viewport;
	int numXCells, numYCells;
	int columns, rows;
	LPCWSTR gridInfo;

public:
	Grid(int width, int height);
	~Grid();


	std::vector<vector<Cell*>> cells;  //Treat 1D vector as 2D vector for cache friendliness
	std::vector<Cell*> activeCells;  //Treat 1D vector as 2D vector for cache friendliness
	
	Cell* GetCell(int x, int y);
	Cell* GetCell(float x, float y);

	void InitObjects(LPGAMEOBJECT obj);
	void InitObjects(LPGAMEOBJECT obj, int cellX, int cellY);

	void Update();

	void GetActiveCells();

	vector<LPGAMEOBJECT> GetActiveObjects();
	void UpdateGridObjects();
	void UpdateObjectGridPosition(LPGAMEOBJECT obj);
};

