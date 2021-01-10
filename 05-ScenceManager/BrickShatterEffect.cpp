#include "BrickShatterEffect.h"

BrickShatterEffect::BrickShatterEffect(float posX, float posY, int _nx, float vyMultiplier = 1.f)
{
	this->SetAnimationSet(CAnimationSetDatabase::GetInstance()->Get(BRICK_SHATTER_EFFECT_ANIMATION_SET));
	x = posX;
	y = posY;
	nx = _nx;
	vx = 0.07f * nx;
	vy = -0.16f * vyMultiplier;
	duration = BRICK_SHATTER_DURATION;
	//isStatic = true;
}

void BrickShatterEffect::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);

	vy += 0.0009f * dt;

	x += dx;
	y += dy;
}

void BrickShatterEffect::Render()
{
	animation_set->at(0)->Render(nx, x, y);
}