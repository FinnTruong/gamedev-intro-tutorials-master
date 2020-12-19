#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Player.h"
#include "Game.h"
#include "PlayScence.h"

#include "Ground.h"
#include "QuestionBlock.h"
#include "OneWayPlatform.h"
#include "Goomba.h"
#include "Paragoomba.h"
#include "KoopaTroopa.h"
#include "Fireball.h"
#include "Mushroom.h"
#include "Brick.h"
#include "PBlock.h"
#include "PSwitch.h"
#include "Portal.h"

#include "PlayerIdleState.h"
#include "PlayerWalkingState.h"
#include "PlayerJumpingState.h"
#include "PlayerFallingState.h"
#include "PlayerAttackingState.h"
#include "PlayerFlyingState.h"


Player* Player::instance = NULL;

Player::Player(float x, float y) : CGameObject()
{
	tag = Tag::PLAYER;
	level = MARIO_LEVEL_BIG;
	untouchable = 0;
	instance = this;
	isFlip = false;
	isAttacking = false;
	abilityBar = 0;

	if (!tail)
	{
		tail = new Tail();
	}

	if (!fireball1)
	{
		fireball1 = new Fireball();
		listFireball.push_back(fireball1);
	}
	if (!fireball2)
	{
		fireball2 = new Fireball();
		listFireball.push_back(fireball2);
	}

	playerState = new PlayerIdleState();
	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
}

Player* Player::GetInstance()
{
	if (instance == NULL)
		instance = new Player();
	
	return instance;
}

void Player::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	playerState->Update(dt);

	CheckCanAttack();

	//Enter Pipe
	if (isEnteringSecretRoom)
	{
		y += MARIO_ENTER_PIPE_SPEED * dt;
		if (y >= 190 && !inSecretRoom)
		{
			SetPosition(2120, 286);
			inSecretRoom = true;
		}
		if (y >= 318)
			isEnteringSecretRoom = false;

		return;
	}

	if (isExitingSecretRoom)
	{
		int marioHeight = level == MARIO_LEVEL_SMALL ? MARIO_SMALL_BBOX_HEIGHT : MARIO_BIG_BBOX_HEIGHT;
		y -= MARIO_ENTER_PIPE_SPEED * dt;
		if (y <= 286 && inSecretRoom)
		{
			SetPosition(2344, 238 - marioHeight);
			inSecretRoom = false;
		}
		if (y <= 206 - marioHeight)
			isExitingSecretRoom = false;
		return;
	}


	// Simple fall down
	if (isSlowFalling && vy > 0)
	{
		vy += MARIO_GRAVITY / 3 * dt;
	}
	else
		vy += MARIO_GRAVITY * dt;
	
	//Check if Mario is kicking object
	isKicking = GetTickCount64() - startKickingAnimationTime <= MARIO_KICKING_TIME;
	

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	

	

	float k, l;
	GetPosition(k, l);
	if (!keyCode[DIK_A])
	{
		holdingObject = nullptr;
	}
	if (holdingObject && keyCode[DIK_A])
	{
		if (level == MARIO_LEVEL_SMALL)
			holdingObject->SetPosition(nx == 1 ? (k + nx * 10) : k + nx * 13, /*holdthing->Gety*/ l - 15);
		else if (level == MARIO_LEVEL_RACCOON)
			holdingObject->SetPosition(nx == 1 ? k + nx * 17 : k + nx * 13, /*holdthing->Gety*/ l - 5);
		else
			holdingObject->SetPosition(nx == 1 ? x + nx * 10 : x + nx * 13, /*holdthing->Gety*/ l - 5);
	}
	HandleCollision(dt, coObjects);
}



void Player::SetState(PlayerState* newState)
{	
	previousState = state;
	delete playerState;
	playerState = newState;
	state = newState->state;	
}

/*
	Reset Mario status to the beginning state of a scene
*/
void Player::Reset()
{
	inSecretRoom = false;
	SetState(new PlayerIdleState());
	SetLevel(MARIO_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void Player::CheckCanAttack()
{
	switch (level)
	{
	case MARIO_LEVEL_RACCOON:
		if (GetTickCount64() - attackTime >= MARIO_RACCOON_ATTACK_TIME)
			isAttacking = false;
		break;

	case MARIO_LEVEL_FIRE:
		if (GetTickCount64() - attackTime >= MARIO_FIRE_ATTACK_TIME)
			isAttacking = false;

	}
}

void Player::HandleMovement(DWORD dt)
{
	float targetVelocity = 0;
	float curVelocity = vx;
	float acceleration = 0;
	float drag = 0;

	bool isRunning = false;

	if (keyCode[DIK_LEFT] || keyCode[DIK_RIGHT])
	{
		if (keyCode[DIK_A])
		{
			isRunning = true;

		}
		else
		{
			isRunning = false;
		}

		if (keyCode[DIK_LEFT])
		{
			targetVelocity = -1 * (isRunning ? MARIO_RUNNING_SPEED : MARIO_WALKING_SPEED);
			nx = -1;
		}
		else if (keyCode[DIK_RIGHT])
		{
			targetVelocity = 1 * (isRunning ? MARIO_RUNNING_SPEED : MARIO_WALKING_SPEED);
			nx = 1;
		}

		if (abs(curVelocity - targetVelocity) - abs(acceleration * dt))
		{
			acceleration = curVelocity < targetVelocity ? MARIO_ACCELERATION : -MARIO_ACCELERATION;
			curVelocity += acceleration * dt;
		}

		vx = curVelocity;
		facingDirection = curVelocity >= 0 ? 1 : -1;
	}
	else
	{
		if (!isGrounded)
			return;		
	}

	
}

#pragma region  Handle Collision

void Player::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_RACCOON || level == MARIO_LEVEL_FIRE)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	else
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
}


void Player::HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{	
	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
	{
		CalcPotentialCollisions(coObjects, coEvents);
		CalcPotentialOverlapped(coObjects);
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->obj->tag != Tag::ONE_WAY_PLATFORM)
			{
				if (nx != 0) vx = 0;
				if (ny != 0) vy = 0;
			}
			//if (ny < 0) isGrounded = true;


			if (e->obj->tag == Tag::ONE_WAY_PLATFORM)
			{

				//Collision from top
				if (e->ny != 0)
				{
					if (e->ny < 0)
					{
						vy = 0;
					}
					else
					{
						y += dy;
					}
				}
			}

			else if (e->obj->tag == Tag::QUESTION_BLOCK)
			{
				QuestionBlock* brick = dynamic_cast<QuestionBlock*>(e->obj);

				if (e->ny > 0)
				{
					brick->SetState(BRICK_STATE_COLLISION);
				}
			}

			else if (e->obj->tag == Tag::P_BLOCK)
			{
				PBlock* brick = dynamic_cast<PBlock*>(e->obj);

				if (e->ny > 0)
				{
					brick->SetState(P_BLOCK_STATE_EMPTY);
				}
			}

			else if (e->obj->tag == Tag::P_SWITCH)
			{
				PSwitch* p_switch = dynamic_cast<PSwitch*>(e->obj);

				if (e->ny < 0)
				{
					if (p_switch->state == SWITCH_STATE_NOT_ACTIVATE)
					{
						p_switch->SetState(SWITCH_STATE_ACTIVATE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
						//Set All brick to coin
						auto scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
						auto obj = scene->GetObjList();
						for (size_t i = 0; i < obj.size(); i++)
						{
							if (obj[i]->tag == Tag::BRICK)
							{
								obj[i]->SetState(1);
							}
						}
					}
				}
			}

			else if (e->obj->tag == Tag::GOOMBA) // if e->obj is Goomba 
			{
				Goomba* goomba = dynamic_cast<Goomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->OnSteppedOn();
						auto pointEffect = new PointEffect(e->obj->x, e->obj->y, 0);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							/*else
								SetState(MARIO_STATE_DIE);*/
						}
					}
				}
			}

			else if (e->obj->tag == Tag::KOOPA)
			{
				KoopaTroopa* koopa = dynamic_cast<KoopaTroopa*>(e->obj);

				if (e->ny < 0)
				{
					if (koopa->GetState() == KOOPA_STATE_WALKING)
					{
						koopa->SetState(KOOPA_STATE_SHELL);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
					else if (koopa->GetState() == KOOPA_STATE_SHELL)
					{
						koopa->SetState(KOOPA_STATE_SPIN);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
						y += dy;
					}
					else if (koopa->GetState() == KOOPA_STATE_SPIN)
					{
						koopa->SetState(KOOPA_STATE_SHELL);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}

				}

				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (koopa->GetState() == KOOPA_STATE_WALKING || koopa->GetState() == KOOPA_STATE_SPIN)
						{
							if (level > MARIO_LEVEL_BIG)
							{
								level = MARIO_LEVEL_BIG;
								StartUntouchable();
							}
							else if (level == MARIO_LEVEL_BIG)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							/*else
							{
								SetState(MARIO_STATE_DIE);
								return;
							}*/
						}
						else
						{
							//isKick = true;
							e->obj->nx = (int)nx;
							if (!keyCode[DIK_A])
							{
								isKicking = true;
								startKickingAnimationTime = GetTickCount64();
								holdingObject = nullptr;
								koopa->isBeingHeld = false;
								koopa->SetState(KOOPA_STATE_SPIN);
							}
							else if (keyCode[DIK_A])
							{
								holdingObject = koopa;
								koopa->isBeingHeld = true;
							}
						}
					}
					else
						x += dx;
				}
			}

			else if (e->obj->tag == Tag::PIPE)
			{
				Pipe* p = dynamic_cast<Pipe*>(e->obj);
				if (e->ny < 0)
				{
					if (p->secretEntrance && keyCode[DIK_DOWN] && !isEnteringSecretRoom && !isExitingSecretRoom)
					{
						isEnteringSecretRoom = true;
					}
				}

				if (e->ny > 0)
				{
					if (p->secretEntrance && keyCode[DIK_UP] && !isEnteringSecretRoom && !isExitingSecretRoom)
					{
						isExitingSecretRoom = true;
					}
				}
			}

			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}		
			
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Player::OnOverlapped(LPGAMEOBJECT other)
{
	//DebugOut(L"Overlap");
	if (dynamic_cast<Item*>(other))
	{
		if (dynamic_cast<Item*>(other)->CheckIsSprouting())
			return;
	}

	switch (other->tag)
	{
	case Tag::ITEM:
		other->SetActive(false);
		other->DisableGameObject();
		break;
	case Tag::LEAF:
		other->SetActive(false);
		other->DisableGameObject();
		SetLevel(MARIO_LEVEL_RACCOON);
		break;
	case Tag::MUSHROOM:
		DebugOut(L"Mushroom\n");
		other->SetActive(false);
		other->DisableGameObject();
		SetLevel(MARIO_LEVEL_BIG);
		break;
	case Tag::BRICK:
		if (other->GetState() == BRICK_STATE_COIN)
		{
			other->SetActive(false);
			other->DisableGameObject();
		}
		break;
	default:
		break;
	}
	

}

#pragma endregion


#pragma region Handle Keyboard

void Player::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	// disable control key when Mario die 
	if (this->GetState() == MARIO_STATE_DIE)
		return;
}

void Player::OnKeyDown(int keyCode)
{

	//Transition from any State
	switch (keyCode)
	{
	case DIK_SPACE:
		if (allow[MARIO_STATE_JUMPING])
		{
			allow[MARIO_STATE_JUMPING] = false;
			this->SetState(new PlayerJumpingState());
		}
		if ((abilityBar >= MARIO_FULL_ABILITY_BAR || isFlying))
		{
			if (level == MARIO_LEVEL_RACCOON)
				SetState(new PlayerFlyingState());
		}
		break;
	case DIK_Q:
		SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_W:
		SetLevel(MARIO_LEVEL_RACCOON);
		break;
	case DIK_E:
		SetLevel(MARIO_LEVEL_FIRE);
		break;
	case DIK_R:
		this->Reset();
		break;
	case DIK_A:
		if (!isAttacking)
		{
			attackTime = GetTickCount64();
			if (level == MARIO_LEVEL_RACCOON)			
			{
				isAttacking = true;
				tail->ActivateGameObject();
				SetState(new PlayerAttackingState());
				return;
			}
			else if (level == MARIO_LEVEL_FIRE)
			{
				isAttacking = true;
				SetState(new PlayerAttackingState());
				for (size_t i = 0; i < listFireball.size(); i++)
				{
					if (listFireball[i]->isDone == true)
					{
						listFireball[i]->Fire(nx, x, y);
						break;
					}
				}
				return;
			}
		}
		break;

	case DIK_1:
		SetPosition(2112, 272 + 12);
		inSecretRoom = true;
		break;
	/*case DIK_DOWN:
		isEnteringSecretRoom = true;
		break;
	case DIK_UP:
		isExitingSecretRoom = true;
		break;*/
	case DIK_P:
		auto scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
		auto obj = scene->GetObjList();
		for (size_t i = 0; i < obj.size(); i++)
		{
			if (obj[i]->tag == Tag::BRICK)
			{
				obj[i]->SetState(1);
			}
		}
		break;
	}
}

void Player::OnKeyUp(int keyCode)
{
	switch (keyCode)
	{
	case DIK_RIGHT:
		releaseMoveInputTime = GetTickCount64();
		facingDirection = 1;
		break;
	case DIK_LEFT:
		releaseMoveInputTime = GetTickCount64();
		facingDirection = -1;
		break;
	case DIK_SPACE:
		releaseJumpInputTime = GetTickCount64();
		break;
	case DIK_A:
		if (holdingObject)
		{
			startKickingAnimationTime = GetTickCount64();
			holdingObject->nx = -nx;
			holdingObject->isBeingHeld = false;
			holdingObject->SetState(KOOPA_STATE_SPIN);
		}
		break;
	default:
		break;
	}
}

#pragma endregion


#pragma region Rendering

void Player::Render()
{
	RenderBoundingBox();

	UpdatePlayerStateAnimation();
	UpdateGlobalAnimation();

	int alpha = 255;
	if (untouchable)
		alpha = 128;

	if (level == MARIO_LEVEL_RACCOON && nx == 1 && currentAnimation != MARIO_ANI_RACCOON_ATTACKING)
		animation_set->at(currentAnimation)->Render(nx, x - 6, y, alpha);	
	else if (nx == -1 && animation_set->at(MARIO_ANI_RACCOON_ATTACKING)->GetCurrentFrame() == 1)
	{
		animation_set->at(currentAnimation)->Render(nx, x - 6, y, alpha);
	}
	else
 		animation_set->at(currentAnimation)->Render(nx, x, y, alpha);

}

void Player::UpdatePlayerStateAnimation()
{
	playerState->UpdateAnimation();
	currentAnimation = playerState->animation;
}

void Player::UpdateGlobalAnimation()
{
	auto animation = currentAnimation;

	switch (level)
	{
	case MARIO_LEVEL_SMALL:
		if (isEnteringSecretRoom || isExitingSecretRoom)
			animation = MARIO_ANI_SMALL_ENTER_PIPE;

		if (holdingObject)
			animation = vx == 0 ? MARIO_ANI_SMALL_IDLE_HOLD : MARIO_ANI_SMALL_MOVING_HOLD;
		else if (isKicking)
			animation = MARIO_ANI_SMALL_KICK;
		break;

	case MARIO_LEVEL_BIG:
		if (isEnteringSecretRoom || isExitingSecretRoom)
			animation = MARIO_ANI_BIG_ENTER_PIPE;

		if (holdingObject)
			animation = vx == 0 ? MARIO_ANI_BIG_IDLE_HOLD : MARIO_ANI_BIG_MOVING_HOLD;
		else if (isKicking)
			animation = MARIO_ANI_BIG_KICK;
		break;

	case MARIO_LEVEL_RACCOON:
		if (isEnteringSecretRoom || isExitingSecretRoom)
			animation = MARIO_ANI_RACCOON_ENTER_PIPE;

		if (holdingObject)
			animation = vx == 0 ? MARIO_ANI_RACCOON_IDLE_HOLD : MARIO_ANI_RACCOON_MOVING_HOLD;
		else if (isKicking)
			animation = MARIO_ANI_RACCOON_KICK;
		break;

	case MARIO_LEVEL_FIRE:
		if (isEnteringSecretRoom || isExitingSecretRoom)
			animation = MARIO_ANI_FIRE_ENTER_PIPE;

		if (holdingObject)
			animation = vx == 0 ? MARIO_ANI_FIRE_IDLE_HOLD : MARIO_ANI_FIRE_MOVING_HOLD;
		else if (isKicking)
			animation = MARIO_ANI_FIRE_KICK;
		break;

	default:
		break;
	}
	currentAnimation = animation;
}

#pragma endregion



