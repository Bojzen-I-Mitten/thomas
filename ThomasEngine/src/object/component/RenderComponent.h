#pragma once
#pragma unmanaged
#include <thomas\object\component\RenderComponent.h>
#pragma managed

#include "../Component.h"
#include "../../resource/Model.h"
#include "../../resource/Material.h"

namespace ThomasEngine
{
	[DisallowMultipleComponent]
	[ExecuteInEditor]
	public ref class RenderComponent : public Component
	{
	private:
		Model^ m_model;

	public:
		RenderComponent() : Component(new thomas::object::component::RenderComponent()) {}

		property Model^ model {
			Model^ get() 
			{
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
				thomas::resource::Material* nptr = ((thomas::object::component::RenderComponent*)nativePtr)->GetMaterial(0);
				Resource^ mat =	ThomasEngine::Resources::FindResourceFromNativePtr(nptr);
				if (mat != nullptr)
					return (Material^)mat;
				else
					return gcnew Material(nptr);
			}
			void set(Material^ value) {
				if (value)
					((thomas::object::component::RenderComponent*)nativePtr)->SetMaterial(0, (thomas::resource::Material*)value->m_nativePtr);
				else
					((thomas::object::component::RenderComponent*)nativePtr)->SetMaterial(0, nullptr);

				OnPropertyChanged("material");
			}
		}

		void Update() override
		{
			((thomas::object::component::RenderComponent*)nativePtr)->Update();
		}
	};
}