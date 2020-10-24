#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

class OneWayPlatform : public CGameObject
{
	int width, height;
public:
	OneWayPlatform(int width = 16, int height = 16);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
