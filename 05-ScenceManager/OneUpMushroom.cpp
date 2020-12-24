#include "OneUpMushroom.h"
#include "PointEffect.h"
#include "Game.h"

OneUpMushroom::OneUpMushroom(float posX, float posY) : Mushroom(posX, posY)
{
	tag = Tag::ONE_UP_MUSHROOM;
}

void OneUpMushroom::OnCollected()
{
	SetActive(false);
	DisableGameObject();
	auto pointEffect = new PointEffect(x, y, 0);
	CGame::GetInstance()->AddScore(1000);
}


