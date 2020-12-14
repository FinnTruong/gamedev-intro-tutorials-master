#include "Cactus.h"

void Cactus::Render()
{
	animation_set->at(CACTUS_ANIMATION)->Render(1, x, y);
}