/*
	Time managment class of the engine. Responsible for delta-time (dt) and FPS calculations.
*/

#pragma once
namespace thomas
{
	class ThomasTime
	{
	public:
		static bool Init();
		static void Update();	

	public:
		static void SetTimescale(const float & timescale);

	public:
		static double GetInitTime();
		static float GetTimescale();
		static float GetDeltaTime();	
		static float GetActualDeltaTime();
		static float GetFrameTime();
		static int GetFPS();

	private:
		static double GetElapsedTime();
	
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
		static float s_StackedDeltaTime;
		static int s_framesPassedSinceFPSUpdate;
	};
}