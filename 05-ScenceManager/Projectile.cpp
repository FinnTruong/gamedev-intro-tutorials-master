#include "Projectile.h"

Projectile::Projectile()
{
	isDone = true;
}

Projectile::~Projectile()
{

}

void Projectile::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void Projectile::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
}

void Projectile::Render()
{
}
