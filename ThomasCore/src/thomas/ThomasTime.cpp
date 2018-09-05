#include "ThomasTime.h"
#include "Common.h"
#include <Windows.h>
#include <ctime>

namespace thomas
{
	double ThomasTime::s_initTime;
	double ThomasTime::s_startTime;
	double ThomasTime::s_timeFreq;
	double ThomasTime::s_DeltaTime;
	float ThomasTime::s_FPS;
	float ThomasTime::s_FrameTime;
	float ThomasTime::s_timescale;
	float ThomasTime::s_FpsUpdateFreq;
	float ThomasTime::s_TimeLeftToUpdateFPS;
	float ThomasTime::s_StackedDeltaTime;
	int ThomasTime::s_framesPassedSinceFPSUpdate;

	bool ThomasTime::Init()
	{
		//Get current performance frequency
		LARGE_INTEGER freq;
		if (!QueryPerformanceFrequency(&freq)) LOG("QueryPerformanceFrequency failed");
		s_timeFreq = double(freq.QuadPart);

		//Init time variables
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);
		s_initTime = double(currentTime.QuadPart);
		s_startTime = double(currentTime.QuadPart);
		s_FPS = 0.f;
		s_timescale = 1.f;
		s_FpsUpdateFreq = 0.7f;
		s_TimeLeftToUpdateFPS = s_FpsUpdateFreq;
		return true;
	}

	//Updates the timestep
	void ThomasTime::Update()
	{
		s_DeltaTime = GetElapsedTime();
		s_TimeLeftToUpdateFPS -= (float)s_DeltaTime;

		//Reset
		if (s_TimeLeftToUpdateFPS <= 0)
		{
			float avgDelta = s_StackedDeltaTime / (float)s_framesPassedSinceFPSUpdate;
			s_FPS = 1.f / avgDelta;
			s_FrameTime = 1000.f / s_FPS;
			s_TimeLeftToUpdateFPS = s_FpsUpdateFreq;
			s_StackedDeltaTime = 0.f;
			s_framesPassedSinceFPSUpdate = 0;
		}
		else
		{
			s_StackedDeltaTime += (float)s_DeltaTime;
			s_framesPassedSinceFPSUpdate++;
		}	
	}

	void ThomasTime::SetTimescale(const float & timescale)
	{
		s_timescale = timescale;
	}

	float ThomasTime::GetDeltaTime()
	{
		return (float)(s_DeltaTime * s_timescale);
	}

	float ThomasTime::GetActualDeltaTime()
	{
		return (float)s_DeltaTime;
	}

	double ThomasTime::GetInitTime()
	{
		return s_initTime;
	}

	int ThomasTime::GetFPS()
	{
		return (int)s_FPS;
	}

	float ThomasTime::GetFrameTime()
	{
		return 1000.f / s_FPS;
	}

	float ThomasTime::GetTimescale()
	{
		return s_timescale;
	}

	double ThomasTime::GetElapsedTime()
	{
		LARGE_INTEGER newTime;
		QueryPerformanceCounter(&newTime);
		double elapsed = double(newTime.QuadPart - s_startTime) / s_timeFreq;
		s_startTime = (double)newTime.QuadPart;
		return elapsed;
	}
}