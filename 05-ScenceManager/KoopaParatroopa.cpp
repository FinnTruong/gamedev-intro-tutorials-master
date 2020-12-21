#include "KoopaParatroopa.h"

KoopaParatroopa::KoopaParatroopa() :KoopaTroopa(0)
{
	tag = Tag::KOOPA;
	type = KoopaTroopaType::KOOPA_GREEN;
	SetState(KOOPA_STATE_JUMPING);
	vx = -KOOPA_WALKING_SPEED;
}
