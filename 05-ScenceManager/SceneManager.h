#pragma once

#include "Scence.h"

class SceneManager
{
	static SceneManager* __instance;
	CScene* currentScene;
public:
	SceneManager();
	~SceneManager();

	static SceneManager* GetInstance();
	void SetScene(CScene* scene) { currentScene = scene; }
	CScene* GetCurrentScene() { return currentScene; }

	void Load();
	void Unload();
	void Update(DWORD dt);
	void Render();
};

