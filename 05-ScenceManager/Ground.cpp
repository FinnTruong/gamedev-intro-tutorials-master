#include "Ground.h"

Ground::Ground(int width, int height)
{
	tag = Tag::GROUND;
	this->width = width;
	this->height = height;
	isTrigger = true;
	isStatic = true;
}

void Ground::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CalcPotentialOverlapped(coObjects);
}

void Ground::Render()
{
	//animation_set->at(0)->Render(nx, x, y);
	RenderBoundingBox();
}

void Ground::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}