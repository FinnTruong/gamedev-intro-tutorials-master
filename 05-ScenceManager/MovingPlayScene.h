#pragma once

#include "PlayScence.h"

#define MOVING_SCENE_TILEMAP_X_OFFSET	0
#define MOVING_SCENE_TILEMAP_Y_OFFSET	-178


class MovingPlayScene : public CPlayScene
{
public:
	MovingPlayScene(int id, LPCWSTR filePath);

	void Update(DWORD dt);
	void Render();
	void Load();

	void DrawEndLevelText();
};
