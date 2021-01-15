#include "ExplodeEffect.h"

ExplodeEffect::ExplodeEffect(float posX, float posY)
{
	this->SetAnimationSet(CAnimationSetDatabase::GetInstance()->Get(EXPLODE_EFFECT_ANIMATION_SET));
	x = posX;
	y = posY;
	duration = EXPLODE_EFFECT_DURATION;
}

void ExplodeEffect::Render()
{
	animation_set->at(0)->Render(1, x, y);
}