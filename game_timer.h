
#ifndef GAME_TIMER_H
#define GAME_TIMER_H

#include <windows.h>

class GameTimer
{
	private:
		float StartTime;
		float AnimTickTime;
		float LastCoef;
		bool Stoped;
	public:
		GameTimer();
		void Start();
		void Stop();
		void SetAnimTime(DWORD T);
		float AnimTime();
		void Reset();
};

#endif