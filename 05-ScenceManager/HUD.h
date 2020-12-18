#pragma once

#include <string>

#define HUD_BACKGROUND_ID -200

class HUD
{
	float startTime;
public:
	void Load();
	void Draw();

	void DrawCard();
	void DrawScore();
	void DrawTimer();
	void DrawTags();
	void DrawSpeedBar();

	std::string NumberToString(int, int);

};

