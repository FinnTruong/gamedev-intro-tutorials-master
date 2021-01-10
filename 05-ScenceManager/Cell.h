#pragma once

#include "GameObject.h"
#include <unordered_set>

class Cell
{
	int xID, yID;

public:
	static const int cellWidth = (int)(SCREEN_WIDTH / 2);
	static const int cellHeight = (int)(SCREEN_HEIGHT / 2);
	Rect rect;
	unordered_set<LPGAMEOBJECT> objects;

	Cell(int xID, int yID);
	~Cell();

	bool IsContain(Rect r);

	void AddObject(LPGAMEOBJECT obj);
	void RemoveObject(LPGAMEOBJECT obj);
};

