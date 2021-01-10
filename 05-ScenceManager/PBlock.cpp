#include "PBlock.h"
#include "Utils.h"
#include "Coin.h"
#include "Mushroom.h"

PBlock::PBlock(float posX, float posY)
{
	x = posX;
	y = posY;
	start_y = posY;
	width = P_BLOCK_WIDTH;
	height = P_BLOCK_HEIGHT;
	tag = Tag::P_BLOCK;
	isEmpty = false;
}

void PBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + P_BLOCK_WIDTH;
	b = y + P_BLOCK_HEIGHT;
}

void PBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);


	x += dx;
	y += dy;
	if (y > start_y)
	{
		y = start_y;
		vy = 0;
		if (!hasSpawn)
		{
			SpawnPSwitch();
			hasSpawn = true;
		}
	}
}

void PBlock::Render()
{

	if (isEmpty)
	{
		animation_set->at(P_BLOCK_ANI_EMPTY)->Render(-nx, x, y);
	}
	else
		animation_set->at(P_BLOCK_ANI_NORMAL)->Render(-nx, x, y);
	RenderBoundingBox();
}

void PBlock::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case P_BLOCK_STATE_EMPTY:
		if (!isEmpty)
		{
			y -= 12;
			vy = 0.05f;
			isEmpty = true;
		}
		break;
	default:
		break;
	}
}

void PBlock::SpawnPSwitch()
{
	DebugOut(L"Spawn");
	p_switch = new PSwitch(x, y);

	p_switch->ActivateGameObject();
	p_switch->SproutOut();
}

void PBlock::OnOverlapped(LPGAMEOBJECT obj)
{
	if (obj->tag == Tag::TAIL)
	{
		SpawnPSwitch();
	}
}