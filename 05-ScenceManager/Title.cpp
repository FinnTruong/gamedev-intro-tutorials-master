#include "Title.h"

void Title::Render()
{
	animation_set->at(0)->Render(1, x, y);
}
