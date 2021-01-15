#pragma once

#include "Projectile.h"

#define VENUS_FIREBALL_SPEED 0.05f
#define FIREBALL_SIZE	6

class VenusFireball : public Projectile
{
public:
	VenusFireball();
	~VenusFireball();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	
	void Render();
	void Fire(D3DXVECTOR2 direction,float x, float y);
};

