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
	DrawBackground();
	DrawCard();
	DrawScore();
	DrawSpeedBar();
	DrawTags();
	DrawTimer();
	DrawLife();
	DrawCoinCollected();
}

void HUD::DrawBackground()
{
	LPDIRECT3DTEXTURE9 tex = CTextureDatabase::GetInstance()->Get(-200);

	int cam_x, cam_y;
	CGame::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition(cam_x, cam_y);

	RECT size;
	size.left = 0;
	size.top = 0;
	size.right = size.left + SCREEN_WIDTH;
	size.bottom = size.top + 100;

	CGame::GetInstance()->Draw(0, (float)cam_x, (float)(cam_y + 192), tex, size.left, size.top, size.right, size.bottom);
}

void HUD::DrawCard()
{
	int cam_x, cam_y;
	CGame::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition(cam_x, cam_y);

	auto sprites = CSpriteDatabase::GetInstance();

	CSprite* hud = sprites->Get(50);
	hud->Draw(1, (float)(cam_x + 10), (float)(cam_y + 192 + 4));

	auto cardCollected = CGame::GetInstance()->GetCards();

	for (int i = 0; i < 3; i++)
	{
		LPSPRITE card = sprites->Get(51 + cardCollected[i]);
		card->Draw(1, (float)(cam_x + 10 + 152 + 12 + i * 24), (float)(cam_y + 192 + 4));
	}
}

void HUD::DrawScore()
{
	int cam_x, cam_y;
	CGame::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition(cam_x, cam_y);

	auto sprites = CSpriteDatabase::GetInstance();
	LPSPRITE num = sprites->Get(0);

	int score = CGame::GetInstance()->GetScore();
	string scoreString = NumberToString(score, 7);
	for (int i = 0; (unsigned)i < scoreString.length(); i++)
	{
		
		if (scoreString[i] == ' ')
			continue;

		for (int j = 0; j < 10; j++)
		{
			if (scoreString[i] == 48 + j)
			{
				num = sprites->Get(j);
			}
		}
		num->Draw(1, (float)(cam_x + 10 + 51 + i * 8), (float)(cam_y + 192 + 4 + 15));
	}
}

void HUD::DrawTags()
{
	int cam_x, cam_y;
	CGame::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition(cam_x, cam_y);

	auto sprites = CSpriteDatabase::GetInstance();

	auto worldTag = sprites->Get(1);
	worldTag->Draw(1, (float)(cam_x + 10 + 36), (float)(cam_y + 192 + 4 + 7));

	auto characterTag = sprites->Get(55);
	characterTag->Draw(1, (float)(cam_x + 10 + 4), (float)(cam_y + 192 + 4 + 15));
}

void HUD::DrawTimer()
{
	int cam_x, cam_y;
	CGame::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition(cam_x, cam_y);

	auto sprites = CSpriteDatabase::GetInstance();
	LPSPRITE num = sprites->Get(0);

	int time = CGame::GetInstance()->GetCurrentScene()->GetTimer();
	string scoreString = NumberToString(time, 3);
	for (int i = 0; (unsigned)i < scoreString.length(); i++)
	{

		if (scoreString[i] == ' ')
			continue;

		for (int j = 0; j < 10; j++)
		{
			if (scoreString[i] == 48 + j)
			{
				num = sprites->Get(j);
			}
		}
		num->Draw(1,(float)(cam_x + 10 + 124 + i * 8), (float)(cam_y + 192 + 4 + 15));
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
		barSegment->Draw(1, (float)(cam_x + 10 + 51 + i * 8), (float)(cam_y + 192 + 4 + 6));
	}

	auto p = Mario->abilityBar >= MARIO_FULL_ABILITY_BAR ? sprites->Get(60) : sprites->Get(59);
	p->Draw(1, (float)(cam_x + 10 + 99), (float)(cam_y + 192 + 4 + 6));
}

void HUD::DrawLife()
{
	int cam_x, cam_y;
	CGame::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition(cam_x, cam_y);

	auto sprites = CSpriteDatabase::GetInstance();

	int lives = CGame::GetInstance()->GetLives();
	auto livesSprite = sprites->Get(lives);
	livesSprite->Draw(1, (float)(cam_x + 42 + 4), (float)(cam_y + 192 + 4 + 15));
}

void HUD::DrawCoinCollected()
{
	int cam_x, cam_y;
	CGame::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition(cam_x, cam_y);

	auto sprites = CSpriteDatabase::GetInstance();
	LPSPRITE num = sprites->Get(0);

	int coinCollected = CGame::GetInstance()->GetCoinCollected();
	string coinString = NumberToString(coinCollected, 2);
	for (int i = 0; (unsigned)i < coinString.length(); i++)
	{
		if (coinString[i] == ' ')
			continue;

		for (int j = 0; j < 10; j++)
		{
			if (coinString[i] == 48 + j)
			{
				num = sprites->Get(j);
			}
		}
		if (coinCollected < 10 && i == 0) continue;
		num->Draw(1, (float)(cam_x + 18 + 124 + i * 8), (float)(cam_y + 184 + 4 + 15));
	}

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
