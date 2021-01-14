#include "Curtain.h"

Curtain::Curtain()
{
	startTime = GetTickCount64();
	//vy = CURTAIN_MOVE_SPEED;
}

void Curtain::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
	x += dx;
	y += dy;

	if (GetTickCount64() - startTime >= TIME_TO_WAIT)
		vy = CURTAIN_MOVE_SPEED;

}

void Curtain::Render()
{
	animation_set->at(0)->Render(1, x, y);
}
