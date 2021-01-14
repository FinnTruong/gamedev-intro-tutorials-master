#include "Background.h"

Background::Background()
{
	startTime = GetTickCount64();
}

Background::~Background()
{

}

void Background::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}


void Background::Render()
{
	int	ani = GetTickCount64() - startTime >= WAIT_TIME ? 1 : 0;		

	if (GetTickCount64() - startTime < WAIT_TIME + FLASH_TIME)
		animation_set->at(ani)->Render(1, x, y);
}
