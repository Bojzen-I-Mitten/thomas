#pragma once
#include <d3d11.h>
#include "DirectXTK/SimpleMath.h"
#include <cmath>
#include <string>
namespace DirectX {
	namespace SimpleMath {
		static float DegreesToRadians(float degree)
		{
			return std::fmodf(degree, 360.0f) * (PI / 180.0f);
		}

		static float RadiansToDegrees(float radian)
		{
			return radian * (180.0f / PI);
		}

		static float Lerp(float from, float to, float amount)
		{
			return from + (to - from) * amount;
		}

		static Vector3 ToEuler(Quaternion rotation)
		{
			float yaw = asin(-2.0f * (rotation.x*rotation.z - rotation.w*rotation.y));
			yaw = RadiansToDegrees(yaw);
			yaw = roundf(yaw * 100000.0f) / 100000.0f;
			float pitch = atan2(2.0f * (rotation.y*rotation.z + rotation.w*rotation.x), rotation.w*rotation.w - rotation.x*rotation.x - rotation.y*rotation.y + rotation.z*rotation.z);
			pitch = RadiansToDegrees(pitch);
			pitch = roundf(pitch * 100000.0f) / 100000.0f;
			float roll = atan2(2.0 * (rotation.x*rotation.y + rotation.w*rotation.z), rotation.w*rotation.w + rotation.w*rotation.x - rotation.y*rotation.y - rotation.z*rotation.z);
			roll = RadiansToDegrees(roll);
			roll = roundf(roll * 100000.0f) / 100000.0f;
			return Vector3(pitch, yaw, roll);
		}
		static Quaternion FromEuler(Vector3 rotation)
		{
			return Quaternion::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);
		}

		static std::string ToString(Vector3& V)
		{
			return "(" + std::to_string(V.x) + "," + std::to_string(V.y) + "," + std::to_string(V.z) + ")";
		}
	}
}

namespace thomas
{
	namespace math = DirectX::SimpleMath;
}