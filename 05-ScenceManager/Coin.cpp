#include "Coin.h"
#include "PointEffect.h"
#include "Utils.h"

Coin::Coin(float posX, float posY) : Item(posX, posY)
{
	this->SetAnimationSet(CAnimationSetDatabase::GetInstance()->Get(ANIMATION_SET_COIN));
	SetActive(true);
	tag = Tag::ITEM;
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
		vy += 0.0005 * dt;
		if (vy > 0 && startY - y <= 10)
		{
			if (!hasPointEffect)
			{
				hasPointEffect = true;
				auto pointEffect = new PointEffect(x, y, 4);
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

	animation_set->at(0)->Render(nx, x, y, 255);
	RenderBoundingBox();
}

void Coin::SproutOut()
{
	Item::SproutOut();
	vy = -0.22;
}
