#pragma once
#pragma unmanaged
#include <thomas\object\component\physics\Collider.h>
#pragma managed
#include "../../Component.h"
#include "Rigidbody.h"
namespace ThomasEditor
{
	ref class Rigidbody;
	[HideInInspector]
	[DisallowMultipleComponent]
	public ref class Collider : public Component
	{
	internal:
		Collider() : Component(nullptr){}
	private:
		Rigidbody^ m_attachedRigidbody;
	public:
		Collider(thomas::object::component::Collider* nativePtr) : Component(nativePtr) {}
		
		property Rigidbody^ attachedRigidbody {
			Rigidbody^ get() { return m_attachedRigidbody; }
			void set(Rigidbody^ value);
			
		}

		property bool isTrigger
		{
			bool get() { return ((thomas::object::component::Collider*)nativePtr)->IsTrigger(); }
			void set(bool value) { ((thomas::object::component::Collider*)nativePtr)->SetTrigger(value); }
		}
	};
}