#pragma once

#pragma unmanaged
#include <thomas\utils\Math.h>
#pragma managed
#include <math.h>
using namespace System::ComponentModel;
using namespace System::Runtime::Serialization;
using namespace System;

namespace ThomasEngine
{
	
	public value class Vector4
	{
	public:

		Vector4(float x, float y, float z, float w) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}
		Vector4(thomas::math::Vector4& V)
		{
			this->x = V.x;
			this->y = V.y;
			this->z = V.z;
			this->w = V.w;
		}

		property float x;
		property float y;
		property float z;
		property float w;
	};
	[SerializableAttribute]
	public value class Color
	{
	public:
		Color(float r, float g, float b, float a) {
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}

		Color(thomas::math::Color C)
		{
			
			this->r = C.R();
			this->g = C.G();
			this->b = C.B();
			this->a = C.A();
		}

		property float r;
		property float g;
		property float b;
		property float a;
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

		Vector3(thomas::math::Vector3 V)
		{
			this->x = V.x;
			this->y = V.y;
			this->z = V.z;
		}
		property float x;
		property float y;
		property float z;
	};
	
	public value class Vector2
	{
	public:

		Vector2(float x, float y) {
			this->x = x;
			this->y = y;
		}
		Vector2(thomas::math::Vector2 V)
		{
			this->x = V.x;
			this->y = V.y;
		}

		property float x;
		property float y;
	};

	public value class Matrix4x4
	{
	internal:
		thomas::math::Matrix ToThomas()
		{
			return thomas::math::Matrix(
				values[0, 0], values[0, 1], values[0, 2], values[0, 3],
				values[1, 0], values[1, 1], values[1, 2], values[1, 3],
				values[2, 0], values[2, 1], values[2, 2], values[2, 3],
				values[3, 0], values[3, 1], values[3, 2], values[3, 3]);
		}
	private:
		array<float, 2>^ values;
	public:

		Matrix4x4(thomas::math::Matrix M)
		{
			values[0,0] = M._11; values[0, 1] = M._12; values[0, 2] = M._13; values[0, 3] = M._14;
			values[1, 0] = M._21; values[1, 1] = M._22; values[1, 2] = M._23; values[1, 3] = M._24;
			values[2, 0] = M._31; values[2, 1] = M._32; values[2, 2] = M._33; values[2, 3] = M._34;
			values[3, 0] = M._41; values[3, 1] = M._42; values[3, 2] = M._43; values[3, 3] = M._44;
		}

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

		Quaternion(thomas::math::Quaternion Q)
		{
			this->x = Q.x;
			this->y = Q.y;
			this->z = Q.z;
			this->w = Q.w;
			
		}

		property Vector3 eulerAngles {
			Vector3 get() {

				return Vector3(thomas::math::ToEuler(thomas::math::Quaternion(x, y, z, w)));
			}
		}
	
		property float x;
		property float y;
		property float z;
		property float w;
	};

}