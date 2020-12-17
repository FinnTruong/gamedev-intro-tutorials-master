#include "PointEffect.h"

PointEffect::PointEffect(float posX, float posY, int _point) : Effect()
{
	this->SetAnimationSet(CAnimationSetDatabase::GetInstance()->Get(POINT_EFFECT_ANIMATION_SET));
	x = posX;
	y = posY;
	duration = POINT_EFFECT_DURATION;
	vy = POINT_EFFECT_MOVE_SPEED;
	point = _point;
}


void PointEffect::Render()
{
	Effect::Render();
	animation_set->at(point)->Render(1, x, y);
}
