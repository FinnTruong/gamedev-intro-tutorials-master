#include "Pipe.h"

Pipe::Pipe(float w, float h, int pipeType, bool hasSecretEntrance)
{
	tag = Tag::PIPE;
	width = w;
	height = h;
	type = pipeType;
	secretEntrance = hasSecretEntrance;
	sortingLayer = 1;
}

void Pipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}

void Pipe::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{

}

void Pipe::Render()
{
	int ani;
	if (type == PipeType::BLACK)
		ani = PIPE_ANI_BLACK_SHORT;
	else
		ani = height <= SHORT_PIPE_HEIGHT ? PIPE_ANI_GREEN_SHORT : PIPE_ANI_GREEN_LONG;

	animation_set->at(ani)->Render(1, x, y);

	RenderBoundingBox();
}