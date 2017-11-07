#pragma once
#include "Common.h"


/**
*Time calss, holds delta time(DT), FPS and timescale.
*Timescale is for slowmotion or speedup.
*/

namespace thomas
{
	class THOMAS_API ThomasTime
	{
	private:
		static double GetElapsedTime();
	public:
		static bool Init();

		static void Update();		///Updates delta time and FPS

		static float GetDeltaTime();		///Delta time
		static float GetActualDeltaTime();
		static double GetInitTime();
		static int GetFPS();
		static float GetFrameTime();

		static void SetTimescale(float timescale);		///The smaller the timescale the slower the time
		static float GetTimescale();

		

	private:
		static double s_initTime;
		static double s_startTime;
		static double s_timeFreq;
		static double s_DeltaTime;
		static float s_FPS;
		static float s_FrameTime;
		static float s_timescale;
		static float s_FpsUpdateFreq;
		static float s_TimeLeftToUpdateFPS;
	public:

	};
}