#pragma once
#pragma unmanaged
#include <thomas\object\component\Transform.h>
#pragma managed

#include "../Component.h"
#include <string>
#include <msclr\marshal_cppstd.h>

namespace ThomasEditor
{
	public ref class Transform : public Component
	{
		thomas::object::component::Transform* nativePtr;
	internal:
		Transform(thomas::object::component::Transform* ptr) : Component((thomas::object::component::Component*)ptr) {
			nativePtr = ptr;
		}
	public:
		Transform() : Component("Transform") {
			nativePtr = new thomas::object::component::Transform();
		}

		property Transform^ parent {
			Transform^ get() {
				if (nativePtr->GetParent())
					return gcnew Transform(nativePtr->GetParent());
				else
					return nullptr;
			}

			void set(Transform^ newParent) {
				nativePtr->SetParent(newParent->nativePtr);
			}
		}

		property List<Transform^>^ children {
			List<Transform^>^ get() {
				std::vector<thomas::object::component::Transform*> nativeChildren = nativePtr->m_children;
				List<Transform^>^ managedChildren = gcnew List<Transform^>(nativeChildren.size());

				for (thomas::object::component::Transform* nativeChild : nativeChildren)
				{
					managedChildren->Add(gcnew Transform(nativeChild));
				}
				return managedChildren;
			}
		}

		void Update() override { nativePtr->Update(); }
	};
}