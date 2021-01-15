#include "Grid.h"
#include "Utils.h"
#include "Game.h"

Grid::Grid(int width, int height)
{
	numXCells = width / Cell::cellWidth;
	numYCells = height / Cell::cellHeight;

	columns = (int)ceil((float)width / (SCREEN_WIDTH >> 1));
	rows = (int)ceil((float)height / (SCREEN_HEIGHT >> 1));

	for (int y = 0; y < rows; ++y)
	{
		auto row = std::vector<Cell*>();
		for (int x = 0; x < columns; ++x)
		{
			row.push_back(new Cell(x, y));
		}
		cells.push_back(row);
	}
}


void Grid::Update()
{
	auto camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	viewport = camera->GetRect();
	GetActiveCells();
	UpdateGridObjects();
}

void Grid::GetActiveCells()
{
	activeCells.clear();
	int startX = (int)(viewport.x / Cell::cellWidth);
	int endX = (int)((viewport.x + SCREEN_WIDTH) / Cell::cellWidth);
	int startY = (int)(viewport.y / Cell::cellHeight);
	int endY = (int)(viewport.y + SCREEN_HEIGHT) / Cell::cellHeight;


	for (int i = startX; i <= endX; ++i)
	{
		if (i<0 || i>columns) continue;
		for (int j = startY; j <= endY; ++j)
		{
			if (j<0 || j > rows) continue;
			activeCells.push_back(cells[j][i]);
		}
	}

}

vector<LPGAMEOBJECT> Grid::GetActiveObjects()
{
	vector<LPGAMEOBJECT> coObjects;

	for (size_t i = 0; i < activeCells.size(); i++)
	{
		auto o = activeCells[i]->objects.begin();
		while (o != activeCells[i]->objects.end())
		{
			if (dynamic_cast<LPGAMEOBJECT>(*o)->isActive)
				coObjects.push_back(*o);
			//UpdateActiveObjects(*o);
			o++;
		}
	}
	return coObjects;
}


void Grid::UpdateGridObjects()
{
	vector<LPGAMEOBJECT> gridObjects = GetActiveObjects();
	/*for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < columns; col++)
		{
			auto o = cells[row][col]->objects.begin();
			while (o != cells[row][col]->objects.end())
			{
				if (dynamic_cast<LPGAMEOBJECT>(*o)->isActive)
					gridObjects.push_back(*o);
				++o;
			}
		}
	}*/



	auto o = gridObjects.begin();
	while (o != gridObjects.end())
	{
		UpdateObjectGridPosition(*o);
		++o;
	}
	
}

void Grid::UpdateObjectGridPosition(LPGAMEOBJECT obj)
{
	/*auto rect = obj->GetRect();
	int oldStartX, oldStartY, oldEndX, oldEndY;
	int startX, startY, endX, endY;

	oldStartX = abs(rect.x / Cell::cellWidth);
	oldEndX = abs((rect.x + rect.width) / Cell::cellWidth);
	oldStartY = abs(rect.y / Cell::cellHeight);
	oldEndY = abs((rect.y + rect.height) / Cell::cellHeight);
	
	startX = abs((rect.x + obj->dx) / Cell::cellWidth);
	endX = abs((rect.x + obj->dx + rect.width) / Cell::cellWidth);
	startY = abs((rect.y + obj->dy) / Cell::cellHeight);
	endY = abs((rect.y + obj->dy + rect.height) / Cell::cellHeight);
	

	if (startX != oldStartX || endX != oldEndX || startY != oldStartY || endY != oldEndY)
	{
		for (int iX = oldStartX; iX <= oldEndX; ++iX)
		{
			for (int iY = oldStartY; iY <= oldEndY; ++iY)
			{
				Cell* c = GetCell(iX, iY);
				if (c->objects.size() >= 1)
					c->RemoveObject(obj);
			}
		}

		for (int iX = startX; iX <= endX; ++iX)
		{
			for (int iY = startY; iY <= endY; ++iY)
			{
				Cell* c = GetCell(iX, iY);
				c->AddObject(obj);
			}
		}
	}*/

	Cell* newCell = GetCell(obj->x, obj->y);
	if (newCell != obj->ownerCell)
	{
		obj->ownerCell->RemoveObject(obj);
		newCell->AddObject(obj);
		obj->ownerCell = newCell;
	}

}

void Grid::InitObjects(LPGAMEOBJECT obj)
{
	/*auto rect = obj->GetRect();
	int startX, startY, endX, endY;

	startX = abs(rect.x / Cell::cellWidth);
	endX = abs((rect.x + rect.width) / Cell::cellWidth);
	startY = abs(rect.y/ Cell::cellHeight);
	endY = abs((rect.y + rect.height)/ Cell::cellHeight);

	for (int iX = startX; iX <= endX; ++iX)
	{
		for (int iY = startY; iY <= endY; ++iY)
		{
			Cell* c = GetCell(iX,iY);
			c->AddObject(obj);
			obj->ownerCells.push_back(c);
		}
	}*/
	if (!obj->isStatic)
	{
		Cell* cell = GetCell(obj->x, obj->y);
		cell->AddObject(obj);
		obj->ownerCell = cell;
	}
}

void Grid::InitObjects(LPGAMEOBJECT obj,int cellX, int cellY)
{
	if (!obj->isStatic)
	{
		Cell* cell = cells[cellX][cellY];
		cell->AddObject(obj);
		obj->ownerCell = cell;
	}
}


Cell* Grid::GetCell(int x, int y)
{
	//Bound Check
	if (x < 0) x = 0;
	if (x >= numXCells) x = numXCells;

	if (y < 0) y = 0;
	if (y >= numYCells) y = numYCells;	

	return cells[y][x]; //Treat 1D vector as 2D vector

}

Cell* Grid::GetCell(float x, float y)
{

	int cellX = (int)(x / Cell::cellWidth);
	int cellY = (int)(y / Cell::cellHeight);

	return GetCell(cellX, cellY);
}

Grid::~Grid()
{

}

