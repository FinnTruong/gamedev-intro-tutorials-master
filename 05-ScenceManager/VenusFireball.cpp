#include "VenusFireball.h"

VenusFireball::VenusFireball(): Projectile()
{
	this->SetAnimationSet(CAnimationSetDatabase::GetInstance()->Get(ANIMATION_SET_FIREBALL));
	tag = Tag::VENUS_FIREBALL;
	isTrigger = true;
	isStatic = true;
}

void VenusFireball::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	//left = right = top = bottom = 0;
	left = x;
	top = y;
	right = x + FIREBALL_SIZE;
	bottom = y + FIREBALL_SIZE;
}


void VenusFireball::Render()
{
	animation_set->at(0)->Render(nx, x, y, 255);
	RenderBoundingBox();
}

void VenusFireball::Fire(D3DXVECTOR2 direction, float posX, float posY)
{
	x = posX;
	y = posY;
	D3DXVec2Normalize(&direction, &direction);
	vx = direction.x * VENUS_FIREBALL_SPEED;
	vy = direction.y * VENUS_FIREBALL_SPEED;
}

D3DXVECTOR2 NormalizeVector2(D3DXVECTOR2 vector)
{
	D3DXVECTOR2 result;
	return result;
}


