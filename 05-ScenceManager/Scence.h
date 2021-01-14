#pragma once

#include <d3dx9.h>
#include "KeyEventHandler.h"
#include "Camera.h"

class CScene
{
protected:
	int id;
	LPCWSTR sceneFilePath;
	
	Camera* camera;

	int timer;
	int maxPlayTime;

public:
	//Scene type: 0: Intro		1: World Map		2:Play Scene			3:Moving Scene
	int sceneType = 0;

	CScene(int id, LPCWSTR filePath);

	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0; 
	virtual void KeyState(BYTE* states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;

	Camera* GetCamera() { return camera;}
	virtual int GetTimer() { return 0; }
};
typedef CScene * LPSCENE;
