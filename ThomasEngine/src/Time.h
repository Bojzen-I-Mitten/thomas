#pragma once
#pragma unmanaged
#include <thomas\ThomasTime.h>
#pragma managed


namespace ThomasEditor
{
	public ref class Time
	{
	public:

		property static float DeltaTime
		{
			float get() { return thomas::ThomasTime::GetDeltaTime(); }
		}
		property static float ActualDeltaTime
		{
			float get() { return thomas::ThomasTime::GetActualDeltaTime(); }
		}

		property static float TimeScale
		{
			float get() { return thomas::ThomasTime::GetTimescale(); }
			void set(float value) { thomas::ThomasTime::SetTimescale(value); }
		}



	private:

	};

}