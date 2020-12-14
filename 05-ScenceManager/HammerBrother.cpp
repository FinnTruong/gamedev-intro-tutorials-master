#include "HammerBrother.h"

HammerBrother::HammerBrother(float x, float y)
{
	vx = HAMMER_BROTHER_SPEED;
	nx = 1;
	start_x = x;
}

void HammerBrother::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	nx = vx >= 0 ? 1 : -1;

	float delta = abs(x - start_x);
	if (vx > 0 && x - start_x >= HAMMER_BROTHER_DISTANCE_WALK_BEFORE_TURN)
	{
		vx *= -1;
	}

	if (vx < 0 && start_x - x >= HAMMER_BROTHER_DISTANCE_WALK_BEFORE_TURN)
	{
		vx *= -1;
	}

	x += dx;
	y += dy;

}

void HammerBrother::Render()
{
	animation_set->at(0)->Render(-nx, x, y);
}