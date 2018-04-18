#pragma once
#pragma unmanaged
#include <thomas\object\component\physics\BoxCollider.h>
#pragma managed
#include "Collider.h"
namespace ThomasEditor
{
	public ref class BoxCollider : public Collider
	{
	internal:

	public:
		BoxCollider() : Collider(new thomas::object::component::BoxCollider()) {}
		
		property Vector3 center
		{
			Vector3 get() { return Vector3(((thomas::object::component::BoxCollider*)nativePtr)->getCenter()); }
			void set(Vector3 value) { ((thomas::object::component::BoxCollider*)nativePtr)->SetCenter(thomas::math::Vector3(value.x, value.y, value.z)); }
		}

		property Vector3 size
		{
			Vector3 get() { return Vector3(((thomas::object::component::BoxCollider*)nativePtr)->getSize()); }
			void set(Vector3 value) { ((thomas::object::component::BoxCollider*)nativePtr)->SetSize(thomas::math::Vector3(value.x, value.y, value.z)); }
		}
	};
}