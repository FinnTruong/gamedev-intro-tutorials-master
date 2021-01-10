#include "VenusFireTrap.h"
#include "Player.h"
#include "VenusFireball.h"
#include "Utils.h"

VenusFireTrap::VenusFireTrap(float posX, float posY, int venusType) : PiranhaPlant(posX, posY)
{
	tag = Tag::VENUS_FIRE_TRAP;
	x = posX;
	y = posY;
	start_x = posX;
	start_y = posY;
	width = VENUS_WIDTH;
	height = VENUS_HEIGHT;
	type = venusType;
	isTrigger = true;
	plantHeight = VENUS_HEIGHT;
}

void VenusFireTrap::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + VENUS_WIDTH;
	bottom = y + VENUS_HEIGHT;
}


void VenusFireTrap::Render()
{
	RenderBoundingBox();
	int ani = VENUS_ANI_IDLE_DOWN_RED;
	if (type == VenusType::VENUS_RED)
	{
		if (IsPlayerAbove())
		{
			ani = !isMoving ? VENUS_ANI_IDLE_UP_RED : VENUS_ANI_MOVING_FACE_UP_RED;
		}
		else
			ani = !isMoving ? VENUS_ANI_IDLE_DOWN_RED : VENUS_ANI_MOVING_FACE_DOWN_RED;
	}
	else if (type == VenusType::VENUS_GREEN)
	{
		if (IsPlayerAbove())
		{
			ani = !isMoving ? VENUS_ANI_IDLE_UP_GREEN : VENUS_ANI_MOVING_FACE_UP_GREEN;
		}
		else
			ani = !isMoving ? VENUS_ANI_IDLE_DOWN_GREEN : VENUS_ANI_MOVING_FACE_DOWN_GREEN;
	}

	animation_set->at(ani)->Render(nx, x, y, 255);
}

void VenusFireTrap::Attack()
{
	hasAttacked = true;
	auto fireball = new VenusFireball();
	fireball->ActivateGameObject();

	float degrees;
	float radians;

	//Calculate fireball direction by degree base on Player position (start from [0,-1] to [0,1])
	float delta = abs(y - Mario->y);
	if (delta <= 24)
		degrees = y < Mario->y ? 105.f : 75.f;
	else
		degrees = y < Mario->y ? 135.f : 45.f;

	//Convert degree to direction Vector2

	degrees *= -nx;
	radians = degrees * PI / 180.f; //Convert degree to radian

	D3DXVECTOR2 direction;
	direction.x = sin(radians);
	direction.y = -cos(radians);

	fireball->Fire(direction, x, y);
}

bool VenusFireTrap::IsPlayerAbove()
{
	return y > Mario->y;
}

