#include "QuestionMarkBrick.h"


QuestionMarkBrick::QuestionMarkBrick(float posX, float posY)
{
	x = posX;
	y = posY;
	initialYPos = posY;
	tag = Tag::QUESTIONMARKBRICK;
	hasCollided = false;

}

void QuestionMarkBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
	if (y >= initialYPos)
	{
		y = initialYPos;
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
			y -= 16;
			vy += 0.05;
			hasCollided = true;
		}
		break;
	default:
		break;
	}
}



void QuestionMarkBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}