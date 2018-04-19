#pragma once
#pragma unmanaged
#include <thomas\object\component\RigidBodyComponent.h>
#pragma managed
#include "../Component.h"

namespace ThomasEditor
{
	[DisallowMultipleComponent]
	public ref class Rigidbody : public Component
	{
	public:
		Rigidbody() : Component(new thomas::object::component::RigidBodyComponent()) {}
		
		property bool IsKinematic 
		{
			bool get() { return ((thomas::object::component::RigidBodyComponent*)nativePtr)->IsKinematic(); }
			void set(bool value) { ((thomas::object::component::RigidBodyComponent*)nativePtr)->SetKinematic(value); }
		}

		property float Mass 
		{
			float get(){ return ((thomas::object::component::RigidBodyComponent*)nativePtr)->GetMass(); }
			void set(float value) { ((thomas::object::component::RigidBodyComponent*)nativePtr)->SetMass(value); }
		}
	};
}