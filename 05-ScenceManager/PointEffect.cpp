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
	int ani = 0;
	switch (point)
	{
	case 100:	ani = 0; break;
	case 200:	ani = 1; break;
	case 400:	ani = 2; break;
	case 800:	ani = 3; break;
	case 1000:	ani = 4; break;
	case 2000:	ani = 5; break;
	case 4000:	ani = 6; break;
	case 8000:	ani = 7; break;
	case 0:		ani = 8; break;

	default:
		ani = 0;
		break;
	}

	animation_set->at(ani)->Render(1, x, y);
}
