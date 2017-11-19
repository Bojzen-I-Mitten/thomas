#pragma once
#pragma unmanaged
#include <thomas\object\component\RenderComponent.h>
#pragma managed

#include "../Component.h"
#include <string>
#include <msclr\marshal_cppstd.h>

namespace ThomasEditor
{
	public ref class RenderComponent : public Component
	{
		thomas::object::component::RenderComponent* nativePtr;
	internal:
		RenderComponent(thomas::object::component::RenderComponent* ptr) : Component((thomas::object::component::Component*)ptr) {
			nativePtr = ptr;
		}
	public:
		RenderComponent() : Component("ModelComponent") {
			nativePtr = new thomas::object::component::RenderComponent();
		}

		void SetModel(String^ name) {
			nativePtr->SetModel(msclr::interop::marshal_as<std::string>(name));
		}

		void Update() override { nativePtr->Update(); }
	};
}