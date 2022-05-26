#include "Timer.h"

void CTimer::Start()
{
	startTime = GetTickCount64();
}

void CTimer::Stop()
{
	startTime = 0;
}

bool CTimer::IsTimeUp()
{
	return (startTime && GetTickCount64() - startTime > limitTime);
}

bool CTimer::IsStopped()
{
	return startTime == 0;
}

ULONGLONG CTimer::GetElapsedTime()
{
	return GetTickCount64() - startTime;
}
