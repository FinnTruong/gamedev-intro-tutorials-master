#include "Tail.h"
#include "Game.h"
#include "PlayScence.h"

Tail::Tail()
{
	tag = Tag::TAIL;
	isTrigger = true;
}

void Tail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + TAIL_WIDTH;
	bottom = top + TAIL_HEIGHT;
}

void Tail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (Mario->nx == 1)
	{
		x = Mario->x + 14;
		y = Mario->y + 15;
	}
	else if (Mario->nx == -1)
	{
		x = Mario->x - 8;
		y = Mario->y + 15;
	}
	if (!Mario->isAttacking)
		DisableGameObject();
}

void Tail::Render()
{
	RenderBoundingBox();
}

void Tail::Attack(float posX, float posY)
{
	x = posX;
	y = posY;
}

