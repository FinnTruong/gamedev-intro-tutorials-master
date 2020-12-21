#include "Coin.h"
#include "PointEffect.h"
#include "Utils.h"
#include "Game.h"

Coin::Coin(float posX, float posY) : Item(posX, posY)
{
	this->SetAnimationSet(CAnimationSetDatabase::GetInstance()->Get(ANIMATION_SET_COIN));
	SetActive(true);
	tag = Tag::COIN;
	x = posX;
	y = posY;
	start_y = posY;
	isTrigger = true;
}

void Coin::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = right = top = bottom = 0;
	/*left = x;
	top = y;
	right = x + 15;
	bottom = y + 15;*/
}

void Coin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	
	if (isSprouting)
	{
		vy += (float)(0.0005 * dt);
		if (vy > 0 && start_y - y <= 10)
		{
			if (!hasPlayedPointEffect)
			{
				hasPlayedPointEffect = true;
				auto pointEffect = new PointEffect(x, y, 4);
				CGame::GetInstance()->AddScore(100);
			}
			isActive = false;
		}
	}

	x += dx;
	y += dy;
}

void Coin::Render()
{
	if (!isActive)
		return;

	int ani = 0;
	if (hiddenCoin)
		ani = SMALL_COIN_ANI_IDLE;
	else
		ani = BIG_COIN_ANI_IDLE;

	animation_set->at(ani)->Render(nx, x, y, 255);
	RenderBoundingBox();
}

void Coin::SproutOut()
{
	Item::SproutOut();
	vy = -0.22f;
	hiddenCoin = true;
}

void Coin::OnCollected()
{
	CGame::GetInstance()->AddCoinCollected();
	SetActive(false);
	DisableGameObject();
	CGame::GetInstance()->AddScore(50);
}

Coin::~Coin()
{

}