#include "Cell.h"
#include "Utils.h"

Cell::Cell(int xID, int yID) : xID(xID), yID(yID)
{
	rect.x = xID * cellWidth;
	rect.y = yID * cellHeight;
	rect.width = cellWidth;
	rect.height = cellHeight;
}


bool Cell::IsContain(Rect r)
{
	return rect.IsContain(r);
}

void Cell::AddObject(LPGAMEOBJECT obj)
{
	DebugOut(L"Add object \n");
	if (!(objects.find(obj) != objects.end()))
	{
		objects.insert(obj);
	}
}

void Cell::RemoveObject(LPGAMEOBJECT obj)
{
	if (objects.find(obj) != objects.end())
	{
		objects.erase(obj);
	}
}
