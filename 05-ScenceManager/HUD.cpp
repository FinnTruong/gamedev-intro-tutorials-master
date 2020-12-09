#include "HUD.h"
#include "Game.h"
#include "Sprites.h"
#include "Textures.h"
#include "GameObject.h"

#include <string>

void HUD::Load()
{

}

void HUD::Draw()
{
	DrawBackground();
	DrawCard();
	DrawScore();
	DrawSpeedBar();
	DrawTags();
	DrawTimer();
}

void HUD::DrawBackground()
{
	LPDIRECT3DTEXTURE9 tex = CTextureDatabase::GetInstance()->Get(-200);

	float cam_x, cam_y;
	CGame::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition(cam_x, cam_y);

	RECT size;
	size.left = 0;
	size.top = 0;
	size.right = size.left + SCREEN_WIDTH;
	size.bottom = size.top + 100;

	CGame::GetInstance()->Draw(0, cam_x, cam_y + 184 + 1, tex, size.left, size.top, size.right, size.bottom);
}

void HUD::DrawCard()
{
	float cam_x, cam_y;
	CGame::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition(cam_x, cam_y);

	auto sprites = CSpriteDatabase::GetInstance();

	CSprite* hud = sprites->Get(50);
	hud->Draw(1, cam_x + 10, cam_y + 184 + 4);

	for (size_t i = 0; i < 3; i++)
	{
		LPSPRITE card = sprites->Get(51);
		card->Draw(1, cam_x + 10 + 152 + 12 + i * 24, cam_y + 184 + 4);
	}
}

void HUD::DrawScore()
{
	float cam_x, cam_y;
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
		num->Draw(1, cam_x + 10 + 51 + i * 8, cam_y + 184 + 4 + 15);
	}
}

void HUD::DrawTags()
{
	float cam_x, cam_y;
	CGame::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition(cam_x, cam_y);

	auto sprites = CSpriteDatabase::GetInstance();

	auto worldTag = sprites->Get(1);
	worldTag->Draw(1, cam_x + 10 + 36, cam_y + 184 + 4 + 7);

	auto characterTag = sprites->Get(55);
	characterTag->Draw(1, cam_x + 10 + 4, cam_y + 184 + 4 + 15);
}

void HUD::DrawTimer()
{
	float cam_x, cam_y;
	CGame::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition(cam_x, cam_y);

	auto sprites = CSpriteDatabase::GetInstance();
	LPSPRITE num = sprites->Get(0);

	int time = 100;
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
		num->Draw(1, cam_x + 10 + 124 + i * 8, cam_y + 184 + 4 + 15);
	}
}

void HUD::DrawSpeedBar()
{
	float cam_x, cam_y;
	CGame::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition(cam_x, cam_y);

	auto sprites = CSpriteDatabase::GetInstance();

	for (int i = 0; i < 6; i++)
	{
		auto barSegment = sprites->Get(57);
		barSegment->Draw(1, cam_x + 10 + 51 + i * 8, cam_y + 184 + 4 + 6);
	}

	auto p = sprites->Get(59);
	p->Draw(1, cam_x + 10 + 99, cam_y + 184 + 4 + 6);
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
