#include "Animations.h"
#include "Utils.h"

CAnimationSetDatabase * CAnimationSetDatabase::__instance = NULL;

void CAnimation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSPRITE sprite = CSpriteDatabase::GetInstance()->Get(spriteId);

	if (sprite == NULL)
	{
		DebugOut(L"[ERROR] Sprite ID %d cannot be found!\n", spriteId);
	}

	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);
	frames.push_back(frame);
}

bool CAnimation::HasAnimationEnded()
{
	return currentFrame == frames.size() - 1;
}

// NOTE: sometimes Animation object is NULL ??? HOW ??? 
void CAnimation::Render(int direction, float x, float y, int alpha)
{
	if (this == nullptr)
		return;

	DWORD now = GetTickCount64();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size())
				currentFrame = 0;
		}
	}

	frames[currentFrame]->GetSprite()->Draw(direction, (int)x, (int)y, alpha);
}

CAnimations * CAnimations::__instance = NULL;

CAnimations * CAnimations::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimations();
	return __instance;
}

void CAnimations::Add(int id, LPANIMATION ani)
{
	animations[id] = ani;
}

LPANIMATION CAnimations::Get(int id)
{
	LPANIMATION ani = animations[id];
	if (ani == NULL)
		DebugOut(L"[ERROR] Failed to find animation id: %d\n", id);
	return ani;
}

void CAnimations::Clear()
{
	for (auto x : animations)
	{
		LPANIMATION ani = x.second;
		delete ani;
	}

	animations.clear();
}

CAnimationSetDatabase::CAnimationSetDatabase()
{

}

CAnimationSetDatabase *CAnimationSetDatabase::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimationSetDatabase();
	return __instance;
}

LPANIMATION_SET CAnimationSetDatabase::Get(unsigned int id)
{
	LPANIMATION_SET ani_set = animation_sets[id];
	if (ani_set == NULL)
		DebugOut(L"[ERROR] Failed to find animation set id: %d\n",id);
	 
	return ani_set;
}

void CAnimationSetDatabase::Add(int id, LPANIMATION_SET ani_set)
{
	animation_sets[id] = ani_set;
}
