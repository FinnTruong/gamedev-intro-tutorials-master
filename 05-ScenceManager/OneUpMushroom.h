#pragma once
#include "Mushroom.h"

class OneUpMushroom : public Mushroom
{
public:
	OneUpMushroom(float, float);
	~OneUpMushroom();

	void OnCollected();

};

