#include <d3dx9.h>
#include <algorithm>


#include "Utils.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"
#include "PlayScence.h"



CGameObject::CGameObject()
{
	x = y = 0;
	start_x = x;
	start_y = y;
	start_x = start_y = 0;
	sortingLayer = 0;
	vx = vy = 0;
	dx = dy = 0;
	state = 0;
	tag = Tag::NONE;
	boundingBox.left = boundingBox.right = boundingBox.top = boundingBox.bottom = 0;
	animation_set = 0;
	dt = 0;
	nx = 1;	
	isActive = true;
	isTrigger = false;
}

void CGameObject::Update(DWORD dt,vector<LPGAMEOBJECT> *coObjects)
{
	this->dt = dt;
	dx = vx*dt;
	dy = vy*dt;
}

void CGameObject::SweptAABB(
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
	float& ny)
{
	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry;
	float t_exit;

	t = -1.0f;			// no collision
	nx = ny = 0;

	//
	// Broad-phase test 
	//

	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	if (br < sl || bl > sr || bb < st || bt > sb) return;


	if (dx == 0 && dy == 0) return;		// moving object is not moving > obvious no collision

	if (dx > 0)
	{
		dx_entry = sl - mr;
		dx_exit = sr - ml;
	}
	else if (dx < 0)
	{
		dx_entry = sr - ml;
		dx_exit = sl - mr;
	}


	if (dy > 0)
	{
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}
	else if (dy < 0)
	{
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}

	if (dx == 0)
	{
		tx_entry = -999999.0f;
		tx_exit = 999999.0f;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	if (dy == 0)
	{
		ty_entry = -99999.0f;
		ty_exit = 99999.0f;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}


	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit) return;

	t = t_entry;

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else
	{
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : ny = 1.0f;
	}

}



/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CGameObject::SweptAABBEx(LPGAMEOBJECT coO)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	coO->GetSpeed(svx, svy);

	float sdx = svx * dt;
	float sdy = svy * dt;

	// (rdx, rdy) is RELATIVE movement distance/velocity 
	float rdx = this->dx - sdx;
	float rdy = this->dy - sdy;

	GetBoundingBox(ml, mt, mr, mb);

	SweptAABB(
		ml, mt, mr, mb,
		rdx, rdy,
		sl, st, sr, sb,
		t, nx, ny
	);

	CCollisionEvent* e = new CCollisionEvent(t, nx, ny, rdx, rdy, coO);
	return e;
}

bool CGameObject::IsOverlapped(LPGAMEOBJECT other) 
{
	D3DXVECTOR2 boundingBoxSize = D3DXVECTOR2((float)(boundingBox.right - boundingBox.left), (float)(boundingBox.bottom - boundingBox.top));
	D3DXVECTOR2 otherBoundingBoxSize = D3DXVECTOR2((float)(other->boundingBox.right - other->boundingBox.left), (float)(other->boundingBox.bottom - other->boundingBox.top));
	return (abs(x - other->x) < (boundingBoxSize.x + otherBoundingBoxSize.x) / 2)
		&& abs(y - other->y) < (boundingBoxSize.y + otherBoundingBoxSize.y) / 2;
	
}

/*
	Calculate potential collisions with the list of colliable objects
	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void CGameObject::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CGameObject::FilterCollision(
	vector<LPCOLLISIONEVENT>& coEvents,
	vector<LPCOLLISIONEVENT>& coEventsResult,
	float& min_tx, float& min_ty,
	float& nx, float& ny, float& rdx, float& rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
		}

		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}

void CGameObject::CalcPotentialOverlapped(vector<LPGAMEOBJECT>* coObjects)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i) == this || !coObjects->at(i)->isTrigger)
			continue;

		if (IsOverlapped(coObjects->at(i)))
		{
			OnOverlapped(coObjects->at(i));
		}
	}
}

void CGameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);

	LPDIRECT3DTEXTURE9 bbox = CTextureDatabase::GetInstance()->Get(ID_TEX_BBOX);

	float l,t,r,b; 

	GetBoundingBox(l, t, r, b);
	boundingBox.left = 0;
	boundingBox.top = 0;
	boundingBox.right = (int)r - (int)l;
	boundingBox.bottom = (int)b - (int)t;

	CGame::GetInstance()->Draw(1, x, y, bbox, boundingBox.left, boundingBox.top, boundingBox.right, boundingBox.bottom, 0);
}


void CGameObject::HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}

void CGameObject::ActivateGameObject()
{
	auto curScene = CGame::GetInstance()->GetCurrentScene();
	if (dynamic_cast<CPlayScene*>(curScene))
	{
		dynamic_cast<CPlayScene*>(curScene)->AddNewGameObject(this);
	}
}

void CGameObject::DisableGameObject()
{
	auto curScene = CGame::GetInstance()->GetCurrentScene();
	if (dynamic_cast<CPlayScene*>(curScene))
	{
		dynamic_cast<CPlayScene*>(curScene)->DisableGameObject(this);
	}
}

bool CGameObject::ObjectInCameraRange()
{
	auto camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	int cam_x = 0, cam_y = 0;
	camera->GetPosition(cam_x, cam_y);
	if (x >= cam_x + SCREEN_WIDTH + 96 || x <= cam_x - 96)
		return false;

	return true;
}

CGameObject::~CGameObject()
{

}