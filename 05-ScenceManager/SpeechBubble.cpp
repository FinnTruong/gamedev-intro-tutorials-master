#include "SpeechBubble.h"

void SpeechBubble::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isVisible && GetTickCount64() - visibleStartTime >= VISIBLE_TIME)
	{
		isVisible = false;
		invisibleStartTime = GetTickCount64();
	}

	if (!isVisible && GetTickCount64() - invisibleStartTime >= INVISIBLE_TIME)
	{
		isVisible = true;
		visibleStartTime = GetTickCount64();
	}


}

void SpeechBubble::Render()
{
	if (!isVisible)
		return;
	animation_set->at(0)->Render(1, x, y);
}