#include "SledgeBro.h"

SledgeBro::SledgeBro(float posX, float posY)
{
	tag = Tag::SLEDGE_BRO;
	start_x = posX;
	nx = -1;
	vx = SLEDGE_BRO_MOVE_SPEED;
	width = SLEDGE_BRO_WIDTH;
	height = SLEDGE_BRO_HEIGHT;
}

void SledgeBro::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}

void SledgeBro::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	HandleCollision(dt, coObjects);

	if (abs(x - start_x) > SLEDGE_BRO_MOVE_DISTANCE && vx > 0)
	{
		nx = 1;
		vx = -SLEDGE_BRO_MOVE_SPEED;
	}
	else if (x < start_x && vx < 0)
	{
		nx = -1;
		vx = SLEDGE_BRO_MOVE_SPEED;
	}

}

void SledgeBro::Render()
{
	if (isActive)
		animation_set->at(0)->Render(nx, x, y);
}

void SledgeBro::HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	//Apply Gravity
	vy += 0.0005f * dt;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);
	CalcPotentialOverlapped(coObjects);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx *= -1;

		if (ny != 0) vy = 0;
;

		
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
