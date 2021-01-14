#include "Goal.h"
#include "Utils.h"
#include "Game.h"

Goal::Goal()
{
	tag = Tag::GOAL;
	isTrigger = true;
	startTypeTime = 0;
}

void Goal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	x += dx;
	y += dy;

	if (GetTickCount64() - startTypeTime >= TIME_BETWEEN_SWITCH && !hasTouched)
	{
		startTypeTime = GetTickCount64();
		switch (curType)
		{
		case GOAL_MUSHROOM:
			curType = GOAL_FLOWER;
			break;
		case GOAL_FLOWER:
			curType = GOAL_STAR;
			break;
		case GOAL_STAR:
			curType = GOAL_MUSHROOM;
			break;
		default:
			curType = GOAL_MUSHROOM;
			break;
		}
	}

}

void Goal::Render()
{
	int ani = 0;
	
	if (!hasTouched)
		ani = GOAL_ANI_RANDOM;
	else
	{
		switch (curType)
		{
		case GOAL_MUSHROOM:
			ani = GOAL_ANI_MUSHROOM;
			break;
		case GOAL_FLOWER:
			ani = GOAL_ANI_FLOWER;
			break;
		case GOAL_STAR:
			ani = GOAL_ANI_STAR;
			break;
		default:
			break;
		}
	}
	animation_set->at(ani)->Render(1, x, y);
}

void Goal::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOAL_STATE_HIT:
		vy = -0.1f;	
		hasTouched = true;
		auto cardCollected = CGame::GetInstance()->GetCards();
		for (int i = 0; i < 3; i++)
		{
			if (cardCollected[i] == 0)
			{
				cardCollected[i] = curType;
				break;
			}			
		}
		break;
	}
}