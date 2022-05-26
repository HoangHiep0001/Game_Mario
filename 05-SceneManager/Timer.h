#pragma once
#include "Windows.h"

class CTimer
{
	ULONGLONG startTime, limitTime;
public:
	CTimer(ULONGLONG limitTime)
	{
		this->startTime = 0;
		this->limitTime = limitTime;
	}

	void Start();
	void Stop();
	bool IsTimeUp();
	bool IsStopped();
	ULONGLONG GetElapsedTime();

	~CTimer() {}
};

