#include "ThomasTime.h"
#include "utils\DebugTools.h"
#include <Windows.h>
#include "Common.h"
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
		LARGE_INTEGER freq;
	//	timeBeginPeriod(1);
		if (!QueryPerformanceFrequency(&freq))LOG("QueryPerformanceFrequency failed!!!!");
		s_timeFreq = double(freq.QuadPart);

		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);
		s_initTime = double(currentTime.QuadPart);
		s_startTime = double(currentTime.QuadPart);
		s_FPS = 0.0f;
		s_timescale = 1.0f;
		s_FpsUpdateFreq = 0.7f;
		s_TimeLeftToUpdateFPS = s_FpsUpdateFreq;

		return true;
	}

	void ThomasTime::Update()
	{

		s_DeltaTime = GetElapsedTime();
		s_TimeLeftToUpdateFPS -= s_DeltaTime;
		if (s_TimeLeftToUpdateFPS <= 0)
		{
			float avgDelta = s_StackedDeltaTime / s_framesPassedSinceFPSUpdate;
			s_FPS = 1.0 / avgDelta;
			s_FrameTime = 1000.0 / s_FPS;
			s_TimeLeftToUpdateFPS = s_FpsUpdateFreq;
			s_StackedDeltaTime = 0;
			s_framesPassedSinceFPSUpdate = 0;
		}else
		{
			s_StackedDeltaTime += s_DeltaTime;
			s_framesPassedSinceFPSUpdate++;
		}
	
	}

	float ThomasTime::GetDeltaTime()
	{
		return s_DeltaTime * s_timescale;
	}

	float ThomasTime::GetActualDeltaTime()
	{
		return s_DeltaTime;
	}

	double ThomasTime::GetInitTime()
	{
		return s_initTime;
	}

	int ThomasTime::GetFPS()
	{
		return s_FPS;
	}

	float ThomasTime::GetFrameTime()
	{
		return 1000.0 / s_FPS;
	}

	void ThomasTime::SetTimescale(float timescale)
	{
		s_timescale = timescale;
	}

	float ThomasTime::GetTimescale()
	{
		return s_timescale;
	}


	double ThomasTime::GetElapsedTime()
	{

		LARGE_INTEGER newTime;
		QueryPerformanceCounter(&newTime);
		double elapsed = double(newTime.QuadPart - s_startTime)/ s_timeFreq;
		s_startTime = newTime.QuadPart;
		return elapsed;
	}

}