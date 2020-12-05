#pragma once
#include "GameObject.h"

#define GOOMBA_LEVEL_GOOMBA			0
#define GOOMBA_LEVEL_PARAGOOMBA		1

#define GOOMBA_BBOX_WIDTH		16
#define GOOMBA_BBOX_HEIGHT		15
#define GOOMBA_BBOX_HEIGHT_DIE	9

#define GOOMBA_WALKING_SPEED 0.05f

#define MAX_DISTANCE_TO_PLAYER		64
#define PARAGOOMBA_ATTACK_COOLDOWN	500
#define PARAGOOMBA_TURN_DELAY		200

#define PARAGOOMBA_LOW_JUMP_SPEED 0.05f
#define PARAGOOMBA_HIGH_JUMP_SPEED 0.15f;

#define GOOMBA_TIME_BEFORE_DISAPPEARED 1000

#define PARAGOOMBA_STATE_WALKING	0
#define PARAGOOMBA_STATE_ATTACKING	1
#define GOOMBA_STATE_WALKING		2
#define GOOMBA_STATE_DIE			3
#define GOOMBA_STATE_DIE_ONESHOT	4

#define GOOMBA_ANI_WALKING				0
#define GOOMBA_ANI_DIE					1
#define GOOMBA_ANI_ONESHOT				2
#define PARAGOOMBA_ANI_WALKING			3
#define PARAGOOMBA_ANI_ATTACKING		4
#define PARAGOOMBA_ANI_WALKING_NO_WING	5
#define PARAGOOMBA_ANI_DIE				6
#define PARAGOOMBA_ANI_ONESHOT			7

class Goomba : public CGameObject
{
protected:
	int type;
	int level;

	bool isGrounded;

	float deadTime;
	float timeElapsed;
	bool hasBeenSteppedOn, hasBeenAttacked;
	
	//Paragoomba Variable
	int numOfLowJump;
	int lowJumpMax = 3;
	float finishAttackTime;
	bool highJump;

public: 	
	Goomba();
	~Goomba();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();

	void SetState(int state);
	void SetLevel(int lv) { level = lv; }

	void HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);

	void OnOverlapped(LPGAMEOBJECT obj);
	void OnSteppedOn();
	void OnAttacked();
	
	void UpdateParagoomba();
	bool IsPlayerInRange();
};

enum GoombaType
{
	GOOMBA,
	PARAGOOMBA
};