#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Player.h"
#include "Game.h"

#include "Ground.h"
#include "OneWayPlatform.h"
#include "Goomba.h"
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
	level = MARIO_LEVEL_BIG;
	untouchable = 0;
	instance = this;
	isFlip = false;
	isAttacking = false;
	abilityBar = 0;

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
	// Simple fall down
	
	vy += MARIO_GRAVITY * dt;




	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state!=MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size()==0)
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

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
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
			else if (dynamic_cast<Ground*>(e->obj)) // if e->obj is CBrick 
			{
				Ground* ground = dynamic_cast<Ground*>(e->obj);

				if (e->ny != 0)
				{
					if (e->ny == -1)
					{						
						//vy = 0;
					}
					else
					{
						y += dy;
					}

				}
			}
			else if (dynamic_cast<OneWayPlatform*>(e->obj))
			{

			}
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void Player::Render()
{
	//int ani = -1;
	//if (state == MARIO_STATE_DIE)
	//	ani = MARIO_ANI_DIE;
	//else 
	//if (level == MARIO_LEVEL_BIG)
	//{
	//	if (vx == 0)
	//	{
	//		ani = MARIO_ANI_BIG_IDLE;
	//	}
	//	else
	//		ani = MARIO_ANI_BIG_WALKING;
	//}
	//else if (level == MARIO_LEVEL_SMALL)
	//{
	//	if (state == MARIO_STATE_JUMP)
	//		ani = MARIO_ANI_SMALL_JUMPING;
	//	else
	//	{
	//	if (vx == 0)
	//	{
	//		/*if (nx>0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
	//		else ani = MARIO_ANI_SMALL_IDLE_LEFT;*/
	//		ani = MARIO_ANI_SMALL_IDLE;
	//	}
	//	else
	//		ani = MARIO_ANI_SMALL_WALKING;
	//	}
	//}

	playerState->UpdateAnimation();
	int alpha = 255;
	if (untouchable)
		alpha = 128;

	animation_set->at(playerState->animation)->Render(nx, x, y, alpha);
	RenderBoundingBox();
}

void Player::SetState(PlayerState* newState)
{	
	//switch (state)
	//{
	//case MARIO_STATE_WALKING_RIGHT:
	//	vx = MARIO_WALKING_SPEED;
	//	nx = 1;
	//	break;
	//case MARIO_STATE_WALKING_LEFT: 
	//	vx = -MARIO_WALKING_SPEED;
	//	nx = -1;
	//	break;
	//case MARIO_STATE_JUMP:
	//	// TODO: need to check if Mario is *current* on a platform before allowing to jump again
	//	vy = -MARIO_JUMP_SPEED_Y;
	//	break; 
	//case MARIO_STATE_IDLE: 
	//	vx = 0;				
	//	break;
	//case MARIO_STATE_SIT:
	//	vx = 0;

	//	break;		
	//case MARIO_STATE_DIE:
	//	vy = -MARIO_DIE_DEFLECT_SPEED;
	//	break;
	//}
	previousState = state;
	delete playerState;
	playerState = newState;
	state = newState->state;
	
}

void Player::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y; 

	if (level==MARIO_LEVEL_BIG || level == MARIO_LEVEL_RACCOON || level == MARIO_LEVEL_FIRE)
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

/*
	Reset Mario status to the beginning state of a scene
*/
void Player::Reset()
{
	SetState(new PlayerIdleState());
	SetLevel(MARIO_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void Player::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	// disable control key when Mario die 
	if (this->GetState() == MARIO_STATE_DIE)
		return;



	

	//if (game->IsKeyDown(DIK_RIGHT))
	//	this->SetState(new PlayerWalkingState());
	//else if (game->IsKeyDown(DIK_LEFT))
	//	this->SetState(new PlayerWalkingState());
	///*else if (game->IsKeyDown(DIK_DOWN))
	//	this->SetState(MARIO_STATE_SIT);*/
	//else
	//	this->SetState(new PlayerIdleState());

}

void Player::OnKeyDown(int keyCode)
{
	//Transition from any State
	switch (keyCode)
	{
	case DIK_SPACE:
		if (allow[MARIO_STATE_JUMPING])
		{
			if ((abilityBar >= MARIO_FULL_ABILITY_BAR || isFlying))
			{
				if (level == MARIO_LEVEL_RACCOON)
					SetState(new PlayerFlyingState());
				return;
			}
			allow[MARIO_STATE_JUMPING] = false;
			this->SetState(new PlayerJumpingState());
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
		if (!isAttacking && (level == MARIO_LEVEL_RACCOON))
			SetState(new PlayerAttackingState());
		break;
	default:
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
	}
}

