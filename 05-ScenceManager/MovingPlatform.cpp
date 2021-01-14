#include "MovingPlatform.h"
#include "Player.h"
#include "Game.h"

MovingPlatform::MovingPlatform()
{
	tag = Tag::MOVING_PLATFORM;
	width = MOVING_PLATFORM_WIDTH;
	height = MOVING_PLATFORM_HEIGHT;
	isStatic = true;
}

MovingPlatform::~MovingPlatform()
{

}

void MovingPlatform::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}

void MovingPlatform::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	
	if (!hasTouched && InCameraRange())
		vx = MOVING_PLATFORM_MOVE_SPEED;
	else if(hasTouched && GetTickCount64() - playerTouchTime >= TIME_BEFORE_FALLING)
	{
		vx = 0;
		vy += MOVING_PLATFORM_FALL_SPEED *dt;
		if (vy >= MOVING_PLATFORM_TERMINAL_VELOCITY)
			vy = MOVING_PLATFORM_TERMINAL_VELOCITY;
	}
	
	HandleCollision(dt, coObjects);
}

void MovingPlatform::Render()
{
	animation_set->at(0)->Render(1, x, y);
	RenderBoundingBox();
}

void MovingPlatform::HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	coEvents.clear();


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
		y += min_ty * dy + ny * 0.8f;

	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

bool MovingPlatform::InCameraRange()
{
	auto camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();

	if (x > camera->x + SCREEN_WIDTH + CAMERA_RANGE_OFFSET || x + width < camera->x)
		return false;

	return true;
}
