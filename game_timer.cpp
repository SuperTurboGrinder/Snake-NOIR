
#include "game_timer.h"

GameTimer::GameTimer() : StartTime(0), AnimTickTime(0), LastCoef(0), Stoped(true)
{  }

void GameTimer::Start()
{
	Stoped = false;
	StartTime = GetTickCount();
}

void GameTimer::Stop()
{
	Stoped = true;
	DWORD currTime = GetTickCount();
	LastCoef = (currTime - StartTime) / AnimTickTime;
}

void GameTimer::SetAnimTime(DWORD T)
{
	AnimTickTime = T;
}

float GameTimer::AnimTime()
{
	if(!Stoped)
	{
		float currTime = GetTickCount();
		float coef = (currTime - StartTime) / AnimTickTime;
		if(coef >= 1)
		{
			Reset();
			Start();
			return 1;
		}

		return coef;
	}
	return LastCoef;
}

void GameTimer::Reset()
{
	LastCoef = 0;
}