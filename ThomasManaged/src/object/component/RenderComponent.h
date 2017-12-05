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
	internal:
		RenderComponent(thomas::object::component::RenderComponent* ptr) {
			nativePtr = ptr;
		}
	public:
		RenderComponent() {
			nativePtr = new thomas::object::component::RenderComponent();
		}
		/*/
		void SetModel(String^ name) {
			nativePtr->SetModel(msclr::interop::marshal_as<std::string>(name));
		}*/

		void Update() override { nativePtr->Update(); }
	};
}