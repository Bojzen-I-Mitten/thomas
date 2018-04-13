#pragma once
#pragma unmanaged
#include <thomas\object\component\RenderComponent.h>
#pragma managed
#include "../Component.h"
#include "../../resource/Model.h"
#include "../../resource/Material.h"

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
				if (m_model == nullptr)
					((thomas::object::component::RenderComponent*)nativePtr)->SetModel(nullptr);
				else
					((thomas::object::component::RenderComponent*)nativePtr)->SetModel((thomas::resource::Model*)value->m_nativePtr);
			}
		}

		property Material^ material {
			Material^ get() {
				return gcnew Material(((thomas::object::component::RenderComponent*)nativePtr)->GetMaterial(0));
			}
			void set(Material^ value) {
				((thomas::object::component::RenderComponent*)nativePtr)->SetMaterial(0, (thomas::resource::Material*)value->m_nativePtr);
			}
		}

		void Update() override
		{
			((thomas::object::component::RenderComponent*)nativePtr)->Update();
		}
	};
}