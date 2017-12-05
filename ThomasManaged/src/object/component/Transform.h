#pragma once
#pragma unmanaged
#include <thomas\object\component\Transform.h>
#pragma managed

#include "../../math/Math.h"
#include "../Component.h"
#include <string>
#include <msclr\marshal_cppstd.h>

using namespace System::ComponentModel;
namespace ThomasEditor
{
	
	public ref class Transform : public Component
	{
	internal:
		Transform(thomas::object::component::Transform* ptr) {
			nativePtr = ptr;
		}
	public:
		Transform() {
			nativePtr = new thomas::object::component::Transform();
		}


		[BrowsableAttribute(false)]
		property Transform^ parent {
			Transform^ get() {
				if (((thomas::object::component::Transform*)nativePtr)->GetParent())
					return gcnew Transform(((thomas::object::component::Transform*)nativePtr)->GetParent());
				else
					return nullptr;
			}

			void set(Transform^ newParent) {
				((thomas::object::component::Transform*)nativePtr)->SetParent((thomas::object::component::Transform*)newParent->nativePtr);
			}
		}
		[BrowsableAttribute(false)]
		property List<Transform^>^ children {
			List<Transform^>^ get() {
				std::vector<thomas::object::component::Transform*> nativeChildren = ((thomas::object::component::Transform*)nativePtr)->m_children;
				List<Transform^>^ managedChildren = gcnew List<Transform^>(nativeChildren.size());

				for (thomas::object::component::Transform* nativeChild : nativeChildren)
				{
					managedChildren->Add(gcnew Transform(nativeChild));
				}
				return managedChildren;
			}
		}

		property Vector3 position
		{
			Vector3 get() { return Vector3(((thomas::object::component::Transform*)nativePtr)->GetPosition()); }
			void set(Vector3 value) { ((thomas::object::component::Transform*)nativePtr)->SetPosition(thomas::math::Vector3(value.x, value.y, value.z));}
		}
		[BrowsableAttribute(false)]
		property Vector3 localPosition
		{
			Vector3 get() { return Vector3(((thomas::object::component::Transform*)nativePtr)->m_localPosition); }
			void set(Vector3 value) { ((thomas::object::component::Transform*)nativePtr)->m_localPosition=thomas::math::Vector3(value.x, value.y, value.z); }
		}
		[BrowsableAttribute(false)]
		property Quaternion rotation
		{
			Quaternion get() { return Quaternion(((thomas::object::component::Transform*)nativePtr)->GetRotation()); }
		}

		
		[DisplayNameAttribute("rotation")]
		property Vector3 eulerAngles
		{
			Vector3 get() { return rotation.eulerAngles; }
			void set(Vector3 value) { ((thomas::object::component::Transform*)nativePtr)->SetRotation(value.x, value.y, value.z); }
		}
		

		
		
		
	};
}