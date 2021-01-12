#include "QuestionBlock.h"
#include "Game.h"
#include "PlayScence.h"
#include "OneUpMushroom.h"

QuestionBlock::QuestionBlock(float posX, float posY, bool _hasPowerup, bool _hasOneUp)
{
	x = posX;
	y = posY;
	startY = posY;
	width = BRICK_BBOX_WIDTH;
	height = BRICK_BBOX_HEIGHT;
	tag = Tag::QUESTION_BLOCK;
	hasCollided = false;
	hasPowerUp = _hasPowerup;
	hasOneUp = _hasOneUp;
	sortingLayer = 1;
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

	CalcPotentialOverlapped(coObjects);

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
	int ani = 0;

	if (hasCollided)
	{
		animation_set->at(1)->Render(-nx, x, y);
	}
	else
	{
		ani = hasOneUp ? 2 : 0;
		animation_set->at(ani)->Render(-nx, x, y);
	}
	RenderBoundingBox();
}

void QuestionBlock::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case QUESTION_BLOCK_STATE_HIT_FROM_BOTTOM:
		if (!hasCollided)
		{
			//Spawn Coin
			if (!hasPowerUp)
				SpawnItem();
			y -= 12;
			vy = 0.05f;
			hasCollided = true;
		}
		break;
	case QUESTION_BLOCK_STATE_HIT_FROM_SIDE:
		if (!hasCollided)
		{
			SpawnItem();
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
	if (hasOneUp)
	{
		item = new OneUpMushroom(x, y);
	}
	else if (hasPowerUp)
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
	{
		item = new Coin(x, y);
		CGame::GetInstance()->AddCoinCollected();
	}

	item->ActivateGameObject();
	item->SproutOut();
}

void QuestionBlock::OnOverlapped(LPGAMEOBJECT obj)
{
	if (obj->tag == Tag::TAIL)
	{
		SetState(QUESTION_BLOCK_STATE_HIT_FROM_SIDE);
	}
}

