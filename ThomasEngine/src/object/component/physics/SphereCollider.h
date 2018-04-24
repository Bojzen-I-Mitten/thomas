#pragma once
#pragma unmanaged
#include <thomas\object\component\physics\SphereCollider.h>
#pragma managed
#include "Collider.h"
namespace ThomasEditor
{
	public ref class SphereCollider : public Collider
	{
	internal:

	public:
		SphereCollider() : Collider(new thomas::object::component::SphereCollider()) {}

		property Vector3 center
		{
			Vector3 get() { return Vector3(((thomas::object::component::SphereCollider*)nativePtr)->getCenter()); }
			void set(Vector3 value) { ((thomas::object::component::SphereCollider*)nativePtr)->SetCenter(thomas::math::Vector3(value.x, value.y, value.z)); }
		}

		property float radius
		{
			float get() { return ((thomas::object::component::SphereCollider*)nativePtr)->GetRadius(); }
			void set(float value) { ((thomas::object::component::SphereCollider*)nativePtr)->SetRadius(value); }
		}
	};
}