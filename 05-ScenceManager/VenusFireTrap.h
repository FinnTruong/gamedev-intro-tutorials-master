#pragma once

#include "PiranhaPlant.h"

#define PI 3.14159265358979323846f

#define VENUS_HEIGHT 32
#define VENUS_WIDTH	 64

#define VENUS_ANI_IDLE_DOWN_RED				0
#define VENUS_ANI_IDLE_UP_RED				1
#define VENUS_ANI_MOVING_FACE_DOWN_RED		2
#define VENUS_ANI_MOVING_FACE_UP_RED		3

#define VENUS_ANI_IDLE_DOWN_GREEN			4
#define VENUS_ANI_IDLE_UP_GREEN				5
#define VENUS_ANI_MOVING_FACE_DOWN_GREEN	6
#define VENUS_ANI_MOVING_FACE_UP_GREEN		7

class VenusFireTrap: public PiranhaPlant
{
	int type;
public:
	VenusFireTrap(float, float, int);
	~VenusFireTrap();

	void Render();
	void Attack();

	bool IsPlayerAbove();
};

enum VenusType
{
	VENUS_RED,
	VENUS_GREEN
};
