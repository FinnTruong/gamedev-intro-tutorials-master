#pragma once

#include "GameObject.h"

#define BLINK_TIME 1000

#define VISIBLE_TIME	725
#define INVISIBLE_TIME	250

class SpeechBubble : public CGameObject
{
	bool isVisible = true;
	ULONGLONG visibleStartTime = 0;
	ULONGLONG invisibleStartTime = 0;
public:
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
};

