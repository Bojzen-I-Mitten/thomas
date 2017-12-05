#pragma once
#include <d3d11.h>
#include "DirectXTK/SimpleMath.h"

namespace DirectX {
	namespace SimpleMath {
		static float DegreesToRadians(float degree)
		{
			return degree * (PI / 180.0f);
		}

		static float RadiansToDegrees(float radian)
		{
			return radian * (180.0f / PI);
		}

		static float Lerp(float from, float to, float amount)
		{
			return from + (to - from) * amount;
		}
	}
}

namespace thomas
{
	namespace math = DirectX::SimpleMath;
}