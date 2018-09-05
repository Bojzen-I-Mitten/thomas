#pragma once
#pragma warning(disable:4561)

#include <d3d11.h>
#include "DirectXTK\SimpleMath.h"
#include <cmath>
#include <string>

namespace DirectX
{
	namespace SimpleMath
	{
		using DirectX::BoundingBox;
		using DirectX::BoundingSphere;
		using DirectX::BoundingFrustum;
		using DirectX::BoundingOrientedBox;

		inline float DegreesToRadians(const float & degree)
		{
			return std::fmodf(degree, 360.f) * (PI / 180.f);
		}

		inline float RadiansToDegrees(const float & radian)
		{
			return radian * (180.f / PI);
		}

		inline float Lerp(const float & from, const float & to, const float & amount)
		{
			return from + (to - from) * amount;
		}

		inline Vector3 ToEuler(const Quaternion & rotation)
		{
			float yaw = asin(-2.f * (rotation.x * rotation.z - rotation.w * rotation.y));
			yaw = RadiansToDegrees(yaw);
			yaw = roundf(yaw * 100000.f) / 100000.f;

			float pitch = atan2f(2.0f * (rotation.y * rotation.z + rotation.w * rotation.x), 
						   rotation.w * rotation.w - rotation.x * rotation.x - rotation.y * rotation.y + rotation.z * rotation.z);
			pitch = RadiansToDegrees(pitch);
			pitch = roundf(pitch * 100000.f) / 100000.f;

			float roll = atan2f(2.f * (rotation.x * rotation.y + rotation.w * rotation.z), 
								rotation.w * rotation.w + rotation.w * rotation.x - rotation.y * rotation.y - rotation.z * rotation.z);
			roll = RadiansToDegrees(roll);
			roll = roundf(roll * 100000.f) / 100000.f;
			return Vector3(pitch, yaw, roll);
		}

		inline Quaternion FromEuler(const Vector3 & rotation)
		{
			return Quaternion::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);
		}

		inline std::string ToString(const Vector3 & v)
		{
			return "(" + std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z) + ")";
		}

		inline Matrix CreateMatrix(const Vector3 & position, const Quaternion & rotation, const Vector3 & scale)
		{
			Matrix rot = Matrix::CreateFromQuaternion(rotation);
			return Matrix::CreateScale(scale) * Matrix::CreateWorld(position, rot.Forward(), rot.Up());
		}
	}
}

namespace thomas
{
	namespace math = DirectX::SimpleMath;
}