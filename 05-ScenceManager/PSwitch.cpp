#include "PSwitch.h"
#include "Utils.h"

PSwitch::PSwitch(float posX, float posY) : Item(posX, posY)
{
	this->SetAnimationSet(CAnimationSetDatabase::GetInstance()->Get(SWITCH_ANIMATION_SET));
	x = posX;
	y = posY;
	tag = Tag::P_SWITCH;
	SetState(SWITCH_STATE_NOT_ACTIVATE);
	sproutSpeed = P_SWITCH_SPROUT_SPEED;
	sproutHeight = P_SWITCH_SPROUT_HEIGHT;
}

void PSwitch::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + SWITCH_WIDTH;
	if (state == SWITCH_STATE_NOT_ACTIVATE)
		bottom = top + SWITCH_NORMAL_HEIGHT;
	else
		bottom = top + SWITCH_ACTIVATE_HEIGHT;
}


void PSwitch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Item::Update(dt, coObjects);

	if (isSprouting)
		return;

	HandleCollision(dt, coObjects);

}

void PSwitch::HandleCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

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

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];



		}
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}


void PSwitch::Render()
{
	int ani = 0;
	if (state == SWITCH_STATE_NOT_ACTIVATE)
		ani = SWITCH_ANI_NOT_ACTIVATE;
	else if (state == SWITCH_STATE_ACTIVATE)
		ani = SWITCH_ANI_ACTIVATE;
	animation_set->at(ani)->Render(1, x, y,255);
	RenderBoundingBox();	
}


void PSwitch::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SWITCH_ANI_NOT_ACTIVATE:
		break;
	case SWITCH_ANI_ACTIVATE:
		SetPosition(x, y + SWITCH_NORMAL_HEIGHT - SWITCH_ACTIVATE_HEIGHT);
		break;
	default:
		break;
	}
}