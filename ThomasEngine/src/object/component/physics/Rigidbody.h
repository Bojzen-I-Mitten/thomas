#pragma once
#pragma unmanaged
#include <thomas\object\component\physics\Rigidbody.h>
#pragma managed
#include "../../Component.h"
#include "../../GameObject.h"
namespace ThomasEngine
{
	[DisallowMultipleComponent]
	public ref class Rigidbody : public Component
	{
	public:
		Rigidbody() : Component(new thomas::object::component::Rigidbody()) {}
		

		void Awake() override;


		void OnDestroy() override;

		property bool IsKinematic 
		{
			bool get() { return ((thomas::object::component::Rigidbody*)nativePtr)->IsKinematic(); }
			void set(bool value) { ((thomas::object::component::Rigidbody*)nativePtr)->SetKinematic(value); }
		}

		property float Mass 
		{
			float get(){ return ((thomas::object::component::Rigidbody*)nativePtr)->GetMass(); }
			void set(float value) { ((thomas::object::component::Rigidbody*)nativePtr)->SetMass(value); }
		}
	};
}