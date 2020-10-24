#include "OneWayPlatform.h"
OneWayPlatform::OneWayPlatform(int width, int height)
{
	this->width = width;
	this->height = height;
}

void OneWayPlatform::Render()
{
	//animation_set->at(0)->Render(nx, x, y);
	RenderBoundingBox();
}

void OneWayPlatform::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}