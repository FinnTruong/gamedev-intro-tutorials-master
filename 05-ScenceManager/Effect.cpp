#include "Effect.h"

Effect::Effect()
{
	ActivateGameObject();
	startTime = GetTickCount64();
}

void Effect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	x += dx;
	y += dy;
	
	if (GetTickCount64() - startTime > duration)
	{
		hasDone = true;
		DisableGameObject();
	}
}

void Effect::Render()
{
	if (hasDone)
		return;
}
