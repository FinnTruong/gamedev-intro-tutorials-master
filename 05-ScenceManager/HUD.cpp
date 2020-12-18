#include "HUD.h"
#include "Game.h"
#include "Sprites.h"
#include "Textures.h"
#include "GameObject.h"
#include "Player.h"

#include <string>

void HUD::Load()
{

}

void HUD::Draw()
{
	DrawCard();
	DrawScore();
	DrawSpeedBar();
	DrawTags();
	DrawTimer();
}



void HUD::DrawCard()
{
	int cam_x, cam_y;
	CGame::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition(cam_x, cam_y);

	auto sprites = CSpriteDatabase::GetInstance();

	CSprite* hud = sprites->Get(50);
	hud->Draw(1, cam_x + 10, cam_y + 192 + 4);

	for (size_t i = 0; i < 3; i++)
	{
		LPSPRITE card = sprites->Get(51);
		card->Draw(1, cam_x + 10 + 152 + 12 + i * 24, cam_y + 192 + 4);
	}
}

void HUD::DrawScore()
{
	int cam_x, cam_y;
	CGame::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition(cam_x, cam_y);

	auto sprites = CSpriteDatabase::GetInstance();
	LPSPRITE num = sprites->Get(0);

	int score = 23451;
	string scoreString = NumberToString(score, 7);
	for (size_t i = 0; i < scoreString.length(); i++)
	{
		
		if (scoreString[i] == ' ')
			continue;

		for (size_t j = 0; j < 10; j++)
		{
			if (scoreString[i] == 48 + j)
			{
				num = sprites->Get(j);
			}
		}
		num->Draw(1, cam_x + 10 + 51 + i * 8, cam_y + 192 + 4 + 15);
	}
}

void HUD::DrawTags()
{
	int cam_x, cam_y;
	CGame::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition(cam_x, cam_y);

	auto sprites = CSpriteDatabase::GetInstance();

	auto worldTag = sprites->Get(1);
	worldTag->Draw(1, cam_x + 10 + 36, cam_y + 192 + 4 + 7);

	auto characterTag = sprites->Get(55);
	characterTag->Draw(1, cam_x + 10 + 4, cam_y + 192 + 4 + 15);
}

void HUD::DrawTimer()
{
	int cam_x, cam_y;
	CGame::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition(cam_x, cam_y);

	auto sprites = CSpriteDatabase::GetInstance();
	LPSPRITE num = sprites->Get(0);

	int time = CGame::GetInstance()->GetCurrentScene()->GetTimer();
	string scoreString = NumberToString(time, 3);
	for (size_t i = 0; i < scoreString.length(); i++)
	{

		if (scoreString[i] == ' ')
			continue;

		for (size_t j = 0; j < 10; j++)
		{
			if (scoreString[i] == 48 + j)
			{
				num = sprites->Get(j);
			}
		}
		num->Draw(1, cam_x + 10 + 124 + i * 8, cam_y + 192 + 4 + 15);
	}
}

void HUD::DrawSpeedBar()
{
	int cam_x, cam_y;
	CGame::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition(cam_x, cam_y);

	auto sprites = CSpriteDatabase::GetInstance();

	for (int i = 0; i < 6; i++)
	{

		auto barSegment = Mario->abilityBar <= MARIO_FULL_ABILITY_BAR / (6 - i) ? sprites->Get(57) : sprites->Get(58);
		barSegment->Draw(1, cam_x + 10 + 51 + i * 8, cam_y + 192 + 4 + 6);
	}

	auto p = Mario->abilityBar >= MARIO_FULL_ABILITY_BAR ? sprites->Get(60) : sprites->Get(59);
	p->Draw(1, cam_x + 10 + 99, cam_y + 192 + 4 + 6);
}

string HUD::NumberToString(int num, int numOfChar)
{
	string numStr = to_string(num);
	int delta = numOfChar - numStr.length();
	for (int i = 0; i < delta; i++)
	{
		numStr = "0" + numStr;
	}
	return numStr;
}
