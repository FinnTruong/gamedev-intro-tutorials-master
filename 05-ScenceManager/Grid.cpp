#include "Grid.h"
#include "Utils.h"

Grid::Grid(int width, int height, int cellSize) : 
	m_width(width),
	m_height(height),
	m_cellSize(cellSize)
{
	m_numXCells = ceil((float)m_width / cellSize);
	m_numYCells = ceil((float)m_height / cellSize);
	DebugOut(L"%d : %d", m_numXCells, m_numYCells);
	

	//Allocate all cells
	m_cells.resize(m_numXCells * m_numYCells);
}

void Grid::AddObj(LPGAMEOBJECT gameObj)
{
	Cell* cell = GetCell(gameObj->x, gameObj->y);
	cell->gameObjs.push_back(gameObj);
}

Cell* Grid::GetCell(int x, int y)
{
	//Bound Check
	if (x < 0) x = 0;
	if (x >= m_numXCells) x = m_numXCells;

	if (y < 0) y = 0;
	if (y >= m_numYCells) x = m_numYCells;

	int index = y * m_numYCells + m_numXCells;

	return &m_cells[index]; //Treat 1D vector as 2D vector

}

Cell* Grid::GetCell(float x, float y)
{
	//Bound Check
	//if (x < 0)
	//	return m_cells[0];
	//if (x >= m_width)
	//	return m_cells[0];

	//if (y < 0)
	//	return m_cells[0];
	//if (y >= m_height)
	//	return m_cells[0];


	int cellX = (int)(x / m_cellSize);
	int cellY = (int)(y / m_cellSize);

	return GetCell(cellX, cellY);
}
