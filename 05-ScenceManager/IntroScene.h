#pragma once

#include "Scence.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Scence.h"
#include "Tilemap.h"

#include "Title.h"


class IntroScene : public CScene
{
protected:
	
	int curOption = 0;

	vector<LPGAMEOBJECT> objects;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);

public:
	IntroScene(int id, LPCWSTR filePath);

	void Load();
	void Update(DWORD dt);
	void Render();
	void Unload();

	void KeyState(BYTE* states) {}
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);

	void DrawBackground();
	void DrawOptions();

};

