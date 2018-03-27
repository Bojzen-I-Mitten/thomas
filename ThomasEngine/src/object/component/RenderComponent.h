#pragma once
#pragma unmanaged
#include <thomas\object\component\RenderComponent.h>
#include <thomas\graphics\Model.h>

#pragma managed
#include "../Component.h"
#include <string>
#include <msclr\marshal_cppstd.h>


namespace ThomasEditor
{
	[DisallowMultipleComponent]
	[ExecuteInEditor]
	public ref class RenderComponent : public Component
	{
	public:
		RenderComponent() : Component(new thomas::object::component::RenderComponent()) {
		}

		property String^ Model {
			String^ get() {
				thomas::graphics::Model* model = ((thomas::object::component::RenderComponent*)nativePtr)->GetModel();
				if (model)
					return gcnew String(model->GetName().c_str());
				else
					return "";
			}
			void set(String^ value)
			{
				thomas::graphics::Model* model = thomas::graphics::Model::GetModelByName(msclr::interop::marshal_as<std::string>(value));
				((thomas::object::component::RenderComponent*)nativePtr)->SetModel(model);
			}
		}

		void Update() override
		{
			((thomas::object::component::RenderComponent*)nativePtr)->Update();
		}
	};
}