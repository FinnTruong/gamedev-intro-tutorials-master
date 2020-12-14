#pragma once

#include "GameObject.h"

#define PIPE_ANI_GREEN_SHORT	 0
#define PIPE_ANI_GREEN_LONG		 1
#define PIPE_ANI_BLACK_SHORT	 2

#define SHORT_PIPE_HEIGHT	32
#define LONG_PIPE_HEIGHT	48

class Pipe: public CGameObject
{
	int type;
	int width, height;

public:
	bool secretEntrance = false;

	Pipe(float, float, int, bool);
	~Pipe();

	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

enum PipeType
{
	GREEN,
	BLACK
};

