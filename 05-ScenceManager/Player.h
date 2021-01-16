#pragma once
#include "GameObject.h"
#include "PlayerState.h"
#include "GameGlobal.h"
#include "KoopaTroopa.h"
#include "Tail.h"
#include "Fireball.h"
#include "PointEffect.h"
#include "MovingPlatform.h"


#define MARIO_WALKING_SPEED			0.1f
#define MARIO_RUNNING_SPEED			0.15f
#define MARIO_ENTER_PIPE_SPEED		0.03f

#define MARIO_DRAG_FORCE	0.00015f

#define MARIO_ACCELERATION			0.000346f
#define MARIO_SKID_ACCELERATION		0.00204f

#define MARIO_WALKING_JUMP_SPEED		0.23f
#define MARIO_RUNNING_JUMP_SPEED		0.26f
#define MARIO_RUNNING_MAX_JUMP_SPEED	0.275f
#define MARIO_JUMP_DEFLECT_SPEED		0.15f
#define MARIO_GRAVITY					0.0004f
#define MARIO_TERMINAL_VELOCITY_Y		1.f
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
#define MARIO_ANI_SMALL_MAX_JUMPING		6
#define MARIO_ANI_SMALL_KICK			7
#define MARIO_ANI_SMALL_IDLE_HOLD		8
#define MARIO_ANI_SMALL_MOVING_HOLD		9
#define MARIO_ANI_SMALL_ENTER_PIPE		10

#define MARIO_ANI_BIG_IDLE				11
#define MARIO_ANI_BIG_WALKING			12
#define MARIO_ANI_BIG_SKIDING			13
#define MARIO_ANI_BIG_RUNNING			14
#define MARIO_ANI_BIG_RUNNING_MAX		15
#define MARIO_ANI_BIG_JUMPING			16
#define MARIO_ANI_BIG_MAX_JUMPING		17
#define MARIO_ANI_BIG_FALLING			18
#define MARIO_ANI_BIG_CROUCHING			19
#define MARIO_ANI_BIG_KICK				20
#define MARIO_ANI_BIG_IDLE_HOLD			21
#define MARIO_ANI_BIG_MOVING_HOLD		22
#define MARIO_ANI_BIG_ENTER_PIPE		23

#define MARIO_ANI_RACCOON_IDLE			24
#define MARIO_ANI_RACCOON_WALKING		25
#define MARIO_ANI_RACCOON_SKIDING		26
#define MARIO_ANI_RACCOON_RUNNING		27
#define MARIO_ANI_RACCOON_RUNNING_MAX	28
#define MARIO_ANI_RACCOON_JUMPING		29
#define MARIO_ANI_RACCOON_MAX_JUMPING	30
#define MARIO_ANI_RACCOON_FALLING		31
#define MARIO_ANI_RACCOON_SLOW_FALLING	32
#define MARIO_ANI_RACCOON_CROUCHING		33
#define MARIO_ANI_RACCOON_FLYING		34
#define MARIO_ANI_RACCOON_ATTACKING		35
#define MARIO_ANI_RACCOON_KICK			36
#define MARIO_ANI_RACCOON_IDLE_HOLD		37
#define MARIO_ANI_RACCOON_MOVING_HOLD	38
#define MARIO_ANI_RACCOON_ENTER_PIPE	39

#define MARIO_ANI_FIRE_IDLE				40
#define MARIO_ANI_FIRE_WALKING			41
#define MARIO_ANI_FIRE_SKIDING			42
#define MARIO_ANI_FIRE_RUNNING			43
#define MARIO_ANI_FIRE_RUNNING_MAX		44
#define MARIO_ANI_FIRE_JUMPING			45
#define MARIO_ANI_FIRE_MAX_JUMPING		46
#define MARIO_ANI_FIRE_FALLING			47
#define MARIO_ANI_FIRE_CROUCHING		48
#define MARIO_ANI_FIRE_ATTACKING		49
#define MARIO_ANI_FIRE_KICK				50
#define MARIO_ANI_FIRE_IDLE_HOLD		51
#define MARIO_ANI_FIRE_MOVING_HOLD		52
#define MARIO_ANI_FIRE_ENTER_PIPE		53



#define MARIO_ANI_DIE				54

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
#define MARIO_MAX_HOLD_TIME			3000

#define TIME_BEFORE_SWITCH_SCENE	3000



class Player : public CGameObject
{
private:
	static Player* instance;
	int untouchable = 0;
	DWORD untouchable_start = 0;
	int alpha = 255;


	float start_x;			// initial position of Mario at scene
	float start_y; 

	float last_y;

	float next_vy;

	int cam_x, cam_y;
public: 
	Player(float x = 0.0f, float y = 0.0f);
	static Player* GetInstance();

	bool canAttack = false;

	bool isFlip = false;
	bool isGrounded = false;
	bool isCrouching = false;
	bool isAttacking = false;
	bool isKicking = false;
	bool isSkiding = false;
	bool isFlying = false;
	bool isFalling = false;
	bool isSlowFalling = false;
	bool isOnMovingPlatform = false;

	bool hasJumped = false;

	bool inSecretRoom = false;
	bool isEnteringSecretRoom = false;
	bool isExitingSecretRoom = false;
	bool isEnteringExitPipe = false;
	bool isExitingExitPipe = false;

	bool isDead = false;
	bool hasHitGoal = false;

	int level = 1;
	int currentAnimation = 0;
	int previousState = 0;

	ULONGLONG releaseMoveInputTime = 0;
	ULONGLONG releaseJumpInputTime = 0;
	ULONGLONG startKickingAnimationTime = 0;
	ULONGLONG attackTime = 0;
	ULONGLONG deadTime = 0;
	ULONGLONG pickUpObjectTime = 0;
	ULONGLONG hitGoalTime = 0;
	int facingDirection = 1;

	float onPipeYPos = 0;

	double abilityBar;

	PlayerState* playerState;
	std::unordered_map<int, bool> allow;

	Tail* tail;

	vector<Fireball*> listFireball;
	Fireball* fireball1;
	Fireball* fireball2;

	KoopaTroopa* holdingObject = nullptr;

	MovingPlatform* platform = nullptr;


	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void OnOverlapped(LPGAMEOBJECT other);



	void SetState(PlayerState* newState);
	void SetLevel(int l);
	void StartUntouchable() { untouchable = 1; untouchable_start = (DWORD)GetTickCount64(); }


	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);


	virtual void Render();
	void UpdatePlayerStateAnimation();
	void UpdateGlobalAnimation();



	void HandleMovement(DWORD dt);
	void TakeDamage();

	void CheckIfHoldingObject();
	void CheckIfCanAttack();
	void CheckIfHasHitGoal();
	void CheckIfDead();
	void CheckIfEnterPipe();
	void CheckUntouchableTimer();
	void CheckIfIsOnMovingPlatform();

	void ResetScene();

};