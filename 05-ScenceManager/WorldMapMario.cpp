#include "WorldMapMario.h"

void WorldMapMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	if (isMoving)
	{
		if (abs(x - startMovingXPos) >= 31 || abs(y - startMovingYPos) >= 31)
		{
			vx = 0;
			vy = 0;
			isMoving = false;
		}
	}

	x += dx;
	y += dy;
}

void WorldMapMario::Render()
{
	animation_set->at(0)->Render(nx, x, y);
}

void WorldMapMario::Move(Vector2 dir)
{
	if (isMoving)
		return;

	isMoving = true;
	startMovingXPos = x;
	startMovingYPos = y;

	if (dir.x == 1)
		vx = WM_MARIO_MOVE_SPEED;
	else if (dir.x == -1)
		vx = -WM_MARIO_MOVE_SPEED;

	if (dir.y == 1)
		vy = -WM_MARIO_MOVE_SPEED;
	else if (dir.y == -1)
		vy = WM_MARIO_MOVE_SPEED;
		
}