#pragma once

#include <thomas\utils\Math.h>
#include <math.h>
using namespace System::ComponentModel;

using namespace System;

namespace ThomasEditor
{
	[TypeConverter(ExpandableObjectConverter::typeid)]
	public value class Vector4
	{
	public:

		Vector4(float x, float y, float z, float w) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		property float x;
		property float y;
		property float z;
		property float w;
	};

	public value class Vector3
	{
	private:
	public:
		
		Vector3(float x, float y, float z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		Vector3(thomas::math::Vector3& V)
		{
			this->x = V.x;
			this->y = V.y;
			this->z = V.z;
		}
		property float x;
		property float y;
		property float z;
	};

	[TypeConverter(ExpandableObjectConverter::typeid)]
	public value class Vector2
	{
	public:

		Vector2(float x, float y) {
			this->x = x;
			this->y = y;
		}

		property float x;
		property float y;
	};


	public value class Matrix4x4
	{
	private:
		array<float, 2>^ values;
	public:
		property float default[int, int]{
			float get(int row, int column) { return values[row, column]; }
			void set(int row, int column, float value){values[row, column] = value;}
		}
	};

	public value class Quaternion
	{
	private:
		static Quaternion identityQuaternion = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
	public:

		Quaternion(float x, float y, float z, float w) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		Quaternion(thomas::math::Quaternion& Q)
		{
			this->x = Q.x;
			this->y = Q.y;
			this->z = Q.z;
			this->w = Q.w;
			
		}

		property Vector3 eulerAngles {
			Vector3 get() {
				float yaw = asin(-2.0f * (x*z - w*y));
				yaw = thomas::math::RadiansToDegrees(yaw);
				yaw = roundf(yaw * 100000.0f) / 100000.0f;
				float pitch = atan2(2.0f * (y*z + w*x), w*w - x*x - y*y + z*z);
				pitch = thomas::math::RadiansToDegrees(pitch);
				pitch = roundf(pitch * 100000.0f) / 100000.0f;
				float roll = atan2(2.0 * (x*y + w*z), w*w + x*x - y*y - z*z);
				roll = thomas::math::RadiansToDegrees(roll);
				roll = roundf(roll * 100000.0f) / 100000.0f;
				return Vector3(pitch,yaw,roll);
			}
		}
	
		property float x;
		property float y;
		property float z;
		property float w;
	};

}