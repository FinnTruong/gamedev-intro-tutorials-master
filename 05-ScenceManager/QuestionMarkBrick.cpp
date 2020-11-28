#include "QuestionMarkBrick.h"
#include "Game.h"
#include "PlayScence.h"

QuestionMarkBrick::QuestionMarkBrick(float posX, float posY, bool hasItem)
{
	x = posX;
	y = posY;
	startY = posY;
	tag = Tag::QUESTIONMARKBRICK;
	hasCollided = false;
}

void QuestionMarkBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}

void QuestionMarkBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;
	if (y > startY)
	{
		y = startY;
		vy = 0;
	}
}

void QuestionMarkBrick::Render()
{

	if (hasCollided)
	{
		animation_set->at(1)->Render(-nx, x, y);
	}
	else
		animation_set->at(0)->Render(-nx, x, y);
	RenderBoundingBox();
}

void QuestionMarkBrick::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BRICK_STATE_COLLISION:
		if (!hasCollided)
		{
			if (hasItem && !hasSpawn)
				SpawnItem();
			y -= 16;
			vy += 0.05;
			hasCollided = true;
		}
		break;
	default:
		break;
	}
}

void QuestionMarkBrick::SpawnItem()
{
	auto curScence = CGame::GetInstance()->GetCurrentScene();
	item = new Mushroom(x, y);
	item->ActivateGameObject();
	item->SproutOut();
}



