#include "MovingPlatform.h"

MovingPlatform::MovingPlatform()
{

}

MovingPlatform::~MovingPlatform()
{

}

void MovingPlatform::GetBoundingBox(float& l, float& t, float& r, float& b)
{

}

void MovingPlatform::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	
	vx = moveSpeed;

	
	HandleCollision(dt, coObjects);
}

void MovingPlatform::Render()
{

}

void MovingPlatform::HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}
