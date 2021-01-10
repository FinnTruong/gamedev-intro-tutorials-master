#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprites.h"
#include "Animations.h"
#include "GameGlobal.h"

#define ID_TEX_BBOX -100		// special texture to draw object bounding box
#define ID_TEX_HUD_BG -200		// special texture to draw HUD background

using namespace std;

class Cell;

class CGameObject; 
typedef CGameObject * LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	
	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL) 
	{ 
		this->t = t; 
		this->nx = nx; 
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj; 
	}

	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};


class CGameObject
{
public:

	float x; 
	float y;

	float start_x;
	float start_y;

	int width, height;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;

	int nx;	 

	int state;

	bool isActive;
	bool isTrigger;

	bool isStatic = false; //Not part of spatial partitioning grid

	int sortingLayer = 1;

	RECT boundingBox;

	Tag tag;

	DWORD dt; 

	LPANIMATION_SET animation_set;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	Cell* ownerCell = nullptr;

public: 
	CGameObject();
	~CGameObject();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {}
	virtual void SetBoundingBox() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	virtual void Render() {}
	virtual void SetState(int state) { this->state = state; }
	virtual void HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);


	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }

	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }

	bool IsActive() { return this->isActive; }
	void SetActive(bool value) { this->isActive = value; }

	bool IsOverlapped(LPGAMEOBJECT other);
	virtual void OnOverlapped(CGameObject* other) {}


	int GetState() { return this->state; }

	void RenderBoundingBox();

	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }


	void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st,
		float sr,
		float sb,
		float& t,
		float& nx,
		float& ny);

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);


	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void CalcPotentialOverlapped(vector<LPGAMEOBJECT>* coObjects);

	void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny,
		float& rdx,
		float& rdy);

	void ActivateGameObject();
	void DisableGameObject();

	bool ObjectInCameraRange();

	Rect GetRect()
	{
		return Rect(x, y, (float)width, (float)height);
	}

};

