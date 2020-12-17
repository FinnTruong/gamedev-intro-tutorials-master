#include "QuestionBlock.h"
#include "Game.h"
#include "PlayScence.h"

QuestionBlock::QuestionBlock(float posX, float posY, bool hasItem)
{
	x = posX;
	y = posY;
	startY = posY;
	tag = Tag::QUESTION_BLOCK;
	hasCollided = false;
}

void QuestionBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}

void QuestionBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;
	if (y > startY)
	{
		y = startY;
		vy = 0;
		if (!hasSpawn && hasPowerUp)
		{
			SpawnItem();
			hasSpawn = true;
		}
	}
}

void QuestionBlock::Render()
{

	if (hasCollided)
	{
		animation_set->at(1)->Render(-nx, x, y);
	}
	else
		animation_set->at(0)->Render(-nx, x, y);
	RenderBoundingBox();
}

void QuestionBlock::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BRICK_STATE_COLLISION:
		if (!hasCollided)
		{
			if (!hasPowerUp)
				SpawnItem();
			y -= 12;
			vy = 0.05;
			hasCollided = true;
		}
		break;
	default:
		break;
	}
}

void QuestionBlock::SpawnItem()
{
	auto curScence = CGame::GetInstance()->GetCurrentScene();
	if (hasPowerUp)
	{
		switch (Mario->level)
		{
		case MARIO_LEVEL_SMALL:
			item = new Mushroom(x, y);
			break;
		case MARIO_LEVEL_BIG:
			item = new Leaf(x, y);
			break;
		default:
			item = new Mushroom(x, y);
			break;
		}
	}
	else
		item = new Coin(x, y);

	item->ActivateGameObject();
	item->SproutOut();
}



