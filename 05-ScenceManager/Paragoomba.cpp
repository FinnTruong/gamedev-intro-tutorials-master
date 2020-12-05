#include "Paragoomba.h"

Paragoomba::Paragoomba() : Goomba()
{
	type = GoombaType::PARAGOOMBA;
	tag = Tag::GOOMBA;
	SetLevel(GOOMBA_LEVEL_PARAGOOMBA);
	SetState(PARAGOOMBA_STATE_WALKING);
}
