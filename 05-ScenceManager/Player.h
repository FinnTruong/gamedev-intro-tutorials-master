#pragma once
#include "GameObject.h"
#include "PlayerState.h"
#include "GameGlobal.h"
#include "Koopas.h"
#include "Fireball.h"


#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.15f

#define MARIO_DRAG_FORCE	0.00015f

#define MARIO_ACCELERATION			0.000346f
#define MARIO_SKID_ACCELERATION		0.00204f

//0.1f
#define MARIO_JUMP_SPEED_Y		0.27f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.0005f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f
#define MARIO_ACCELERATION_MULTIPLIER	 0.005f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING			100
#define MARIO_STATE_SKIDDING		200
#define MARIO_STATE_RUNNING			300
#define MARIO_STATE_RUNNING_MAX		400
#define MARIO_STATE_JUMPING			500
#define MARIO_STATE_FALLING			600
#define MARIO_STATE_DIE				700
#define MARIO_STATE_CROUCHING		800
#define MARIO_STATE_FLYING			900
#define MARIO_STATE_ATTACKING		1000


#define MARIO_ANI_SMALL_IDLE			0
#define MARIO_ANI_SMALL_WALKING			1
#define MARIO_ANI_SMALL_SKIDING			2
#define MARIO_ANI_SMALL_RUNNING			3
#define MARIO_ANI_SMALL_RUNNING_MAX		4
#define MARIO_ANI_SMALL_JUMPING			5
#define MARIO_ANI_SMALL_KICK			6
#define MARIO_ANI_SMALL_IDLE_HOLD		7
#define MARIO_ANI_SMALL_MOVING_HOLD		8

#define MARIO_ANI_BIG_IDLE				9
#define MARIO_ANI_BIG_WALKING			10
#define MARIO_ANI_BIG_SKIDING			11
#define MARIO_ANI_BIG_RUNNING			12
#define MARIO_ANI_BIG_RUNNING_MAX		13
#define MARIO_ANI_BIG_JUMPING			14
#define MARIO_ANI_BIG_FALLING			15
#define MARIO_ANI_BIG_CROUCHING			16
#define MARIO_ANI_BIG_KICK				17
#define MARIO_ANI_BIG_IDLE_HOLD			18
#define MARIO_ANI_BIG_MOVING_HOLD		19

#define MARIO_ANI_RACCOON_IDLE			20
#define MARIO_ANI_RACCOON_WALKING		21
#define MARIO_ANI_RACCOON_SKIDING		22
#define MARIO_ANI_RACCOON_RUNNING		23
#define MARIO_ANI_RACCOON_RUNNING_MAX	24
#define MARIO_ANI_RACCOON_JUMPING		25
#define MARIO_ANI_RACCOON_FALLING		26
#define MARIO_ANI_RACCOON_SLOW_FALLING	27
#define MARIO_ANI_RACCOON_CROUCHING		28
#define MARIO_ANI_RACCOON_FLYING		29
#define MARIO_ANI_RACCOON_ATTACKING		30
#define MARIO_ANI_RACCOON_KICK			31
#define MARIO_ANI_RACCOON_IDLE_HOLD		32
#define MARIO_ANI_RACCOON_MOVING_HOLD	33

#define MARIO_ANI_FIRE_IDLE				34
#define MARIO_ANI_FIRE_WALKING			35
#define MARIO_ANI_FIRE_SKIDING			36
#define MARIO_ANI_FIRE_RUNNING			37
#define MARIO_ANI_FIRE_RUNNING_MAX		38
#define MARIO_ANI_FIRE_JUMPING			39
#define MARIO_ANI_FIRE_FALLING			40
#define MARIO_ANI_FIRE_CROUCHING		41
#define MARIO_ANI_FIRE_ATTACKING		42
#define MARIO_ANI_FIRE_KICK				43
#define MARIO_ANI_FIRE_IDLE_HOLD		44
#define MARIO_ANI_FIRE_MOVING_HOLD		45




#define MARIO_ANI_DIE				46

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define	MARIO_LEVEL_RACCOON	3
#define	MARIO_LEVEL_FIRE	4

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_UNTOUCHABLE_TIME 5000

#define MARIO_FULL_ABILITY_BAR 1200

#define MARIO_RACCOON_ATTACK_TIME	300
#define MARIO_FIRE_ATTACK_TIME		200
#define MARIO_KICKING_TIME			300


class Player : public CGameObject
{
private:
	static Player* instance;
	int untouchable;
	DWORD untouchable_start;


	float start_x;			// initial position of Mario at scene
	float start_y; 
public: 
	Player(float x = 0.0f, float y = 0.0f);
	static Player* GetInstance();


	bool isFlip;
	bool isGrounded;
	bool isAttacking;
	bool isKicking;
	bool isFlying;
	bool isSlowFalling;

	int level;
	int currentAnimation;
	int previousState;

	float releaseMoveInputTime;
	float releaseJumpInputTime;
	float startKickingAnimationTime;
	float attackTime;
	int facingDirection;

	float abilityBar;

	PlayerState* playerState;
	std::unordered_map<int, bool> allow;

	vector<Fireball*> listFireball;
	Fireball* fireball1;
	Fireball* fireball2;

	Koopa* holdingObject = nullptr;


	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void HandleMovement(DWORD dt);
	void HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);

	void OnOverlapped(LPGAMEOBJECT other);

	void SetState(PlayerState* newState);
	void SetLevel(int l) { level = l; }

	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void CheckCanAttack();

	void Reset();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);

	void UpdatePlayerStateAnimation();
	void UpdateGlobalAnimation();
	
};