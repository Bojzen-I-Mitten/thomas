#pragma once
#pragma unmanaged
#include <thomas\object\component\RenderComponent.h>
#pragma managed
#include "../Component.h"
#include "../../resource/Model.h"

namespace ThomasEditor
{
	[DisallowMultipleComponent]
	[ExecuteInEditor]
	public ref class RenderComponent : public Component
	{
	private:
		Model^ m_model;
	public:
		RenderComponent() : Component(new thomas::object::component::RenderComponent()) {
		}

		property Model^ model {
			Model^ get() {
				return m_model;
				
			}
			void set(Model^ value)
			{
				m_model = value;
				((thomas::object::component::RenderComponent*)nativePtr)->SetModel((thomas::resource::Model*)value->m_nativePtr);
			}
		}

		void Update() override
		{
			((thomas::object::component::RenderComponent*)nativePtr)->Update();
		}
	};
}