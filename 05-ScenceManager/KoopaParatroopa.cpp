#include "KoopaParatroopa.h"

KoopaParatroopa::KoopaParatroopa(float x, float y,int type) : KoopaTroopa(x,y,type)
{
	this->start_y = y;
	tag = Tag::KOOPA;
	this->type = type;
	if (type == KoopaTroopaType::KOOPA_RED)
	{
		SetState(KOOPA_STATE_FLYING);
		vy = KOOPA_FLYING_SPEED;
	}
	else
	{
		SetState(KOOPA_STATE_JUMPING);
		vx = -KOOPA_WALKING_SPEED;
	}
}
