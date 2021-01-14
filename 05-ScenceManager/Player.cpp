#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Player.h"
#include "Game.h"
#include "PlayScence.h"
#include "MovingPlayScene.h"

#include "Ground.h"
#include "QuestionBlock.h"
#include "OneWayPlatform.h"
#include "MovingPlatform.h"
#include "Goomba.h"
#include "Paragoomba.h"
#include "KoopaTroopa.h"
#include "Fireball.h"
#include "Mushroom.h"
#include "OneUpMushroom.h"
#include "Brick.h"
#include "PBlock.h"
#include "PSwitch.h"
#include "Coin.h"
#include "Portal.h"

#include "PlayerIdleState.h"
#include "PlayerWalkingState.h"
#include "PlayerJumpingState.h"
#include "PlayerFallingState.h"
#include "PlayerAttackingState.h"
#include "PlayerFlyingState.h"


Player* Player::instance = NULL;

Player::Player(float posX, float posY) : CGameObject()
{
	tag = Tag::PLAYER;
	isDead = false;
	sortingLayer = 1;
	level = MARIO_LEVEL_SMALL;
	width = MARIO_SMALL_BBOX_WIDTH;
	height = MARIO_SMALL_BBOX_HEIGHT;
	untouchable = 0;
	instance = this;
	isFlip = false;
	isAttacking = false;
	abilityBar = 0;
	isStatic = true;

	if (!tail)
	{
		tail = new Tail();
	}

	if (!fireball1)
	{
		fireball1 = new Fireball();
		fireball1->ActivateGameObject();
		listFireball.push_back(fireball1);
	}
	if (!fireball2)
	{
		fireball2 = new Fireball();
		fireball2->ActivateGameObject();
		listFireball.push_back(fireball2);
	}

	playerState = new PlayerIdleState();
	start_x = posX; 
	start_y = posY; 
	last_y = posY;
	this->x = posX; 
	this->y = posY; 

}

Player* Player::GetInstance()
{
	if (instance == NULL)
		instance = new Player();
	
	return instance;
}

void Player::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGame::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition(cam_x, cam_y);

	CGameObject::Update(dt);
	playerState->Update(dt);

	HandleCollision(dt, coObjects);

	CheckIfHoldingObject();
	CheckIfCanAttack();
	CheckIfHasHitGoal();
	CheckIfDead();
	CheckIfEnterPipe();	
	CheckUntouchableTimer();
}


// -------------------------- HANDLE COLLISION --------------------------------


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
	else if(level == MARIO_LEVEL_SMALL)
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
	
	if (state == MARIO_STATE_DIE)
		left = right = top = bottom = 0;
}


void Player::HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{	
	if (isEnteringSecretRoom || isExitingSecretRoom || isEnteringExitPipe || isExitingExitPipe)
		return;

	if (isSlowFalling && vy > 0)
	{
		vy += MARIO_GRAVITY / 3 * dt;
	}
	else
		vy += MARIO_GRAVITY * dt;


	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);
	CalcPotentialOverlapped(coObjects);


	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;		
		if ((int)y > (int)last_y)
		{
			isOnMovingPlatform = false;
			isFalling = true;
		}
		else
		{			
			isFalling = false;
		}
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


		//if (vy >= MARIO_GRAVITY * dt || vy < 0)
		//{
		//	isGrounded = false;
		//}
		if (ny < 0)
		{
			isFalling = false;
			isGrounded = true;
			hasJumped = false;
		}


		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->obj->tag != Tag::ONE_WAY_PLATFORM && e->obj->tag != Tag::COIN && e->obj->tag != Tag::MUSHROOM && e->obj->tag != Tag::ONE_UP_MUSHROOM)
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
					brick->SetState(QUESTION_BLOCK_STATE_HIT_FROM_BOTTOM);
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
						auto obj = scene->GetBrickList();
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
						auto pointEffect = new PointEffect(e->obj->x, e->obj->y, 100);
						CGame::GetInstance()->AddScore(100);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goomba->state != GOOMBA_STATE_DIE)
						{
							TakeDamage();
							return;
						}

					}
				}
			}

			else if (e->obj->tag == Tag::KOOPA)
			{
				KoopaTroopa* koopa = dynamic_cast<KoopaTroopa*>(e->obj);

				if (e->ny < 0)
				{
					koopa->OnSteppedOn();
					auto pointEffect = new PointEffect(e->obj->x, e->obj->y, 100);
					CGame::GetInstance()->AddScore(100);
					vy = -MARIO_JUMP_DEFLECT_SPEED;
					
				}

				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (koopa->GetState() == KOOPA_STATE_FLYING || koopa->GetState() == KOOPA_STATE_JUMPING || koopa->GetState() == KOOPA_STATE_WALKING || koopa->GetState() == KOOPA_STATE_SPIN)
						{
							TakeDamage();
							return;
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
								pickUpObjectTime = GetTickCount64();
								holdingObject = koopa;
								koopa->isBeingHeld = true;
							}
						}
					}
					else
						x += dx;
				}
			}

			else if (e->obj->tag == Tag::MUSHROOM)
			{
				auto mushroom = dynamic_cast<Mushroom*>(e->obj);
				if (e->nx != 0 || e->ny != 0)
				{
					mushroom->OnCollected();
					SetLevel(MARIO_LEVEL_BIG);
				}
			}

			else if (e->obj->tag == Tag::ONE_UP_MUSHROOM)
			{
				auto mushroom = dynamic_cast<OneUpMushroom*>(e->obj);
				if (e->nx != 0 || e->ny != 0)
				{
					mushroom->OnCollected();
				}
			}

			else if (e->obj->tag == Tag::COIN)
			{
				auto coin = dynamic_cast<Coin*>(e->obj);
				if (e->nx != 0 || e->ny != 0)
				{
					coin->OnCollected();
				}
			}


			else if (e->obj->tag == Tag::PIPE)
			{
				Pipe* p = dynamic_cast<Pipe*>(e->obj);
				if (e->ny < 0)
				{
					auto curScene = CGame::GetInstance()->GetCurrentScene();
					if (curScene->sceneType != 3)
					{
						if (p->secretEntrance && keyCode[DIK_DOWN] && !isEnteringSecretRoom && !isExitingSecretRoom)
						{
							isEnteringSecretRoom = true;
							onPipeYPos = y;
						}
					}
					else if (curScene->sceneType == 3)
					{
						if (p->secretEntrance && keyCode[DIK_DOWN] && !isEnteringExitPipe)
						{
							isEnteringExitPipe = true;
							onPipeYPos = y;								
						}
					}
				}

				if (e->ny > 0)
				{
					if (p->secretEntrance && keyCode[DIK_UP] && !isEnteringSecretRoom && !isExitingSecretRoom)
					{
						isExitingSecretRoom = true;
						onPipeYPos = y;
					}
				}
			}

			else if (e->obj->tag == Tag::MOVING_PLATFORM)
			{
				platform = dynamic_cast<MovingPlatform*>(e->obj);
				if (e->ny < 0)
				{
					
					if (!platform->hasTouched)
						platform->playerTouchTime = GetTickCount64();

					isOnMovingPlatform = true;
					platform->hasTouched = true;					
					if (platform != nullptr)
					{
						if (x > platform->x + platform->width || x + width < platform->x)
						{
							isFalling = true;
							if (vy >= MARIO_TERMINAL_VELOCITY_Y)
								vy = 0;
						}
						else
							vy += platform->dy;
					}
				}
				else
				{
					vy = 0;
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
	last_y = y;

	if (platform != nullptr)
	{
		if (x > platform->x + platform->width || x + width < platform->x)
		{
			if (vy >=MARIO_TERMINAL_VELOCITY_Y)
				vy = 0;
		}
	}

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
	case Tag::PIRANHA_PLANT: case Tag::VENUS_FIRE_TRAP:
		if (dynamic_cast<PiranhaPlant*>(other)->canDamagePlayer)
			TakeDamage();
		break;
	case Tag::VENUS_FIREBALL:
		TakeDamage();
		break;
	case Tag::ITEM:
		dynamic_cast<Item*>(other)->OnCollected();
		break;
	case Tag::LEAF:
		dynamic_cast<Item*>(other)->OnCollected();
		SetLevel(MARIO_LEVEL_RACCOON);
		break;
	case Tag::COIN:
		dynamic_cast<Coin*>(other)->OnCollected();
		break;
	case Tag::BRICK:
		if (other->GetState() == BRICK_STATE_COIN)
		{
			other->SetActive(false);
			other->DisableGameObject();
		}
		break;
	case Tag::GOAL:
		Goal* goal = dynamic_cast<Goal*>(other);
		if (!goal->hasTouched)
		{
			goal->SetState(GOAL_STATE_HIT);
			hasHitGoal = true;
			hitGoalTime = GetTickCount64();
			vx = 0;
		}
		break;
	}


}


#pragma endregion



// ---------------------------- HANDLE KEYBOARD -----------------------------------


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
		if (!hasJumped)
		{
			if (isGrounded && !isFalling)
			{
				hasJumped = true;
				allow[MARIO_STATE_JUMPING] = false;
				float jumpSpeed = 0;
				switch (state)
				{
				case MARIO_STATE_IDLE: case MARIO_STATE_WALKING:
					jumpSpeed = MARIO_WALKING_JUMP_SPEED;
					break;
				case MARIO_STATE_RUNNING:
					jumpSpeed = MARIO_RUNNING_JUMP_SPEED;
					break;
				case MARIO_STATE_RUNNING_MAX:
					jumpSpeed = MARIO_RUNNING_MAX_JUMP_SPEED;
					break;
				default:
					jumpSpeed = MARIO_WALKING_JUMP_SPEED;
					break;
				}
				this->SetState(new PlayerJumpingState(jumpSpeed));
			}
		}
		else if ((abilityBar >= MARIO_FULL_ABILITY_BAR || isFlying))
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
		this->ResetScene();
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
	case DIK_2:
		SetPosition(2272, -66 - 32);
		CGame::GetInstance()->GetCurrentScene()->GetCamera()->cameraFolowYAxis = true;
		break;
	case DIK_M:
		CGame::GetInstance()->SwitchScene(4);
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



// -------------------------- RENDERING -------------------------------


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

	if (state == MARIO_STATE_DIE)
	{
		currentAnimation = MARIO_ANI_DIE;
		return;
	}

	switch (level)
	{
	case MARIO_LEVEL_SMALL:
		if (isEnteringSecretRoom || isExitingSecretRoom || isEnteringExitPipe || isExitingExitPipe)
			animation = MARIO_ANI_SMALL_ENTER_PIPE;

		else if (holdingObject)
			animation = vx == 0 ? MARIO_ANI_SMALL_IDLE_HOLD : MARIO_ANI_SMALL_MOVING_HOLD;
		else if (isKicking)
			animation = MARIO_ANI_SMALL_KICK;

		else if (isSkiding && isGrounded)
			animation = MARIO_ANI_SMALL_SKIDING;

		else if (isFalling)
			animation = abilityBar >= MARIO_FULL_ABILITY_BAR ? MARIO_ANI_SMALL_MAX_JUMPING : MARIO_ANI_SMALL_JUMPING;
		break;

	case MARIO_LEVEL_BIG:
		if (isEnteringSecretRoom || isExitingSecretRoom || isEnteringExitPipe || isExitingExitPipe)
			animation = MARIO_ANI_BIG_ENTER_PIPE;

		else if (holdingObject)
			animation = vx == 0 ? MARIO_ANI_BIG_IDLE_HOLD : MARIO_ANI_BIG_MOVING_HOLD;
		else if (isKicking)
			animation = MARIO_ANI_BIG_KICK;
		else if (isSkiding && isGrounded)
			animation = MARIO_ANI_BIG_SKIDING;
		else if (isFalling)
			animation = abilityBar >= MARIO_FULL_ABILITY_BAR ? MARIO_ANI_BIG_MAX_JUMPING : MARIO_ANI_BIG_FALLING;
		break;

	case MARIO_LEVEL_RACCOON:
		if (isEnteringSecretRoom || isExitingSecretRoom || isEnteringExitPipe || isExitingExitPipe)
			animation = MARIO_ANI_RACCOON_ENTER_PIPE;

		else if (holdingObject)
			animation = vx == 0 ? MARIO_ANI_RACCOON_IDLE_HOLD : MARIO_ANI_RACCOON_MOVING_HOLD;
		else if (isKicking)
			animation = MARIO_ANI_RACCOON_KICK;
		else if (isSkiding && isGrounded)
			animation = MARIO_ANI_RACCOON_SKIDING;
		else if (isFalling && !isSlowFalling)
			animation = abilityBar >= MARIO_FULL_ABILITY_BAR ? MARIO_ANI_RACCOON_MAX_JUMPING : MARIO_ANI_RACCOON_FALLING;
		break;

	case MARIO_LEVEL_FIRE:
		if (isEnteringSecretRoom || isExitingSecretRoom || isEnteringExitPipe || isExitingExitPipe)
			animation = MARIO_ANI_FIRE_ENTER_PIPE;

		else if (holdingObject)
			animation = vx == 0 ? MARIO_ANI_FIRE_IDLE_HOLD : MARIO_ANI_FIRE_MOVING_HOLD;
		else if (isKicking)
			animation = MARIO_ANI_FIRE_KICK;
		else if (isSkiding && isGrounded)
			animation = MARIO_ANI_FIRE_SKIDING;
		else if (isFalling)
			animation = abilityBar >= MARIO_FULL_ABILITY_BAR ? MARIO_ANI_FIRE_MAX_JUMPING : MARIO_ANI_FIRE_FALLING;
		break;

	default:
		break;
	}
	currentAnimation = animation;
}


#pragma endregion



// -------------------------  PLAYER CONTROLLER LOGIC  ----------------------------


#pragma region Player Controller Logic


void Player::HandleMovement(DWORD dt)
{
	if (state == MARIO_STATE_DIE || hasHitGoal)
		return;

	float targetVelocity = 0;
	float curVelocity = vx;
	float acceleration = 0.000146f;
	float drag = 0.0004f;

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

		if (abs(curVelocity - targetVelocity) >= abs(acceleration))
		{
			acceleration = curVelocity < targetVelocity ? MARIO_ACCELERATION : -MARIO_ACCELERATION;
			curVelocity += acceleration * dt;
		}

		vx = curVelocity;
		facingDirection = curVelocity >= 0 ? 1 : -1;
	}
	else
	{
		auto vel = 0;
		if (abs(curVelocity) >= drag * dt)
		{
			curVelocity = abs(curVelocity) - (drag * dt);
		}
		else
			curVelocity = 0.0f;
		vx = facingDirection * curVelocity;
	}

	isSkiding = targetVelocity * facingDirection < 0 ? true : false;
}


void Player::TakeDamage()
{
	if (untouchable != 0)
		return;

	if (level > MARIO_LEVEL_BIG)
	{
		SetLevel(MARIO_LEVEL_BIG);
		StartUntouchable();
	}
	else if (level == MARIO_LEVEL_BIG)
	{
		SetLevel(MARIO_LEVEL_SMALL);
		StartUntouchable();
	}
	else if (state != MARIO_STATE_DIE)
	{
		state = MARIO_STATE_DIE;
		vy = -0.2f;
		vx = 0.0f;
		isDead = true;
		deadTime = GetTickCount64();
		return;
	}
}


#pragma endregion



// ------------------------ SET MARIO VARIABLES -------------------------


#pragma region Set Mario Properties


void Player::SetLevel(int l)
{
	if (level == MARIO_LEVEL_SMALL && l != MARIO_LEVEL_SMALL)
	{
		SetPosition(x, y - (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT));
	}

	if (l == MARIO_LEVEL_SMALL && level != MARIO_LEVEL_SMALL)
	{
		SetPosition(x, y + (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT));
	}

	level = l;
}


void Player::SetState(PlayerState* newState)
{
	if (state == MARIO_STATE_DIE)
		return;
	previousState = state;
	delete playerState;
	playerState = newState;
	state = newState->state;
}


#pragma endregion



// ----------------------  CHECK GAME CONDITIONS  ------------------------


#pragma region Check Game Conditions

void Player::CheckIfHoldingObject()
{
	//Check if Mario is kicking object
	isKicking = GetTickCount64() - startKickingAnimationTime <= MARIO_KICKING_TIME;

	float k, l;
	GetPosition(k, l);
	if (!keyCode[DIK_A])
	{
		holdingObject = nullptr;
	}
	if (holdingObject && keyCode[DIK_A])
	{
		if (level == MARIO_LEVEL_SMALL)
			holdingObject->SetPosition(nx == 1 ? (x + nx * 9) : x + nx * 12, y - 5);
		else if (level == MARIO_LEVEL_RACCOON)
			holdingObject->SetPosition(nx == 1 ? x + nx * 10 : x + nx * 13, y + 4);
		else
			holdingObject->SetPosition(nx == 1 ? x + nx * 10 : x + nx * 13, y + 4);
	}

	if (holdingObject && GetTickCount64() - pickUpObjectTime >= MARIO_MAX_HOLD_TIME)
	{
		auto koopa = dynamic_cast<KoopaTroopa*>(holdingObject);
		koopa->isBeingHeld = false;
		koopa->SetState(KOOPA_STATE_WALKING);
		koopa->SetPosition(koopa->x + nx * 5, koopa->y - koopa->height);
		holdingObject = nullptr;
	}
}


void Player::CheckIfCanAttack()
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


void Player::CheckIfHasHitGoal()
{
	if (hasHitGoal)
	{
		if (isGrounded)
			vx = MARIO_WALKING_SPEED;
		nx = 1;
		if (GetTickCount64() - hitGoalTime >= TIME_BEFORE_SWITCH_SCENE)
		{
			CGame::GetInstance()->SwitchScene(4);
			return;
		}
		/*if (x >= cam_x + SCREEN_WIDTH + 32)
		{
			CGame::GetInstance()->SwitchScene(4);
			hasHitGoal = true;
			return;
		}*/
	}
}


void Player::CheckIfDead()
{
	if (isDead)
	{
		if (GetTickCount64() - deadTime >= 1500)
		{
			isDead = false;
			CGame::GetInstance()->SwitchScene(4);
			CGame::GetInstance()->SubtractLives();
			return;
		}
	}
}


void Player::CheckIfEnterPipe()
{
	//Enter Pipe
	if (isEnteringSecretRoom)
	{
		int marioHeight = level == MARIO_LEVEL_SMALL ? MARIO_SMALL_BBOX_HEIGHT : MARIO_BIG_BBOX_HEIGHT;
		y += MARIO_ENTER_PIPE_SPEED * dt;
		if (abs(onPipeYPos - y) >= marioHeight && !inSecretRoom)
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
			SetPosition((float)2344, (float)206);
			inSecretRoom = false;
		}
		if (y <= 206 - marioHeight)
			isExitingSecretRoom = false;
		return;
	}

	if (isEnteringExitPipe)
	{
		int marioHeight = level == MARIO_LEVEL_SMALL ? MARIO_SMALL_BBOX_HEIGHT : MARIO_BIG_BBOX_HEIGHT;
		y += MARIO_ENTER_PIPE_SPEED * dt;
			
			
		if (abs(onPipeYPos - y) >= marioHeight)
		{	
			isEnteringExitPipe = false;
			isExitingExitPipe = true;
			SetPosition((float)2208 + 8, (float)206);	
			CGame::GetInstance()->GetCurrentScene()->GetCamera()->cameraFollowXAxis = true;
			return;
		}
	}

	if (isExitingExitPipe)
	{
		int marioHeight = level == MARIO_LEVEL_SMALL ? MARIO_SMALL_BBOX_HEIGHT : MARIO_BIG_BBOX_HEIGHT;
		y -= MARIO_ENTER_PIPE_SPEED * dt;

		if (y < 206 - marioHeight)
		{
			isExitingExitPipe = false;
			return;
		}
	}

}


void Player::CheckUntouchableTimer()
{
	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
}

#pragma endregion



// ------------------------- UTILITY FUNCTIONS ---------------------------

	
  /* Reset Mario status to the beginning state of a scene */
void Player::ResetScene()
{
	inSecretRoom = false;
	state = 0;
	SetState(new PlayerIdleState());
	SetLevel(MARIO_LEVEL_SMALL);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}
