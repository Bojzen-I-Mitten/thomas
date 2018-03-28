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
	[DisallowMultipleComponent]
	[ExecuteInEditor]
	public ref class Transform : public Component
	{

	public:
		Transform() : Component(new thomas::object::component::Transform()) {
		}

		[BrowsableAttribute(false)]
		property Transform^ parent {
			Transform^ get() {
				if (((thomas::object::component::Transform*)nativePtr)->GetParent())
					return (Transform^)GetObject(((thomas::object::component::Transform*)nativePtr)->GetParent());
				else
					return nullptr;
			}

			void set(Transform^ newParent) {
				if (newParent)
				{
					((thomas::object::component::Transform*)nativePtr)->SetParent((thomas::object::component::Transform*)newParent->nativePtr);
				}
				else
				{
					((thomas::object::component::Transform*)nativePtr)->SetParent(nullptr);
				}
				
			}
		}
		[BrowsableAttribute(false)]
		property List<Transform^>^ children {
			List<Transform^>^ get() {
				std::vector<thomas::object::component::Transform*> nativeChildren = ((thomas::object::component::Transform*)nativePtr)->m_children;
				List<Transform^>^ managedChildren = gcnew List<Transform^>(nativeChildren.size());

				for (thomas::object::component::Transform* nativeChild : nativeChildren)
				{
					
					managedChildren->Add((Transform^)GetObject(nativeChild));
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
			Vector3 get() { return Vector3(((thomas::object::component::Transform*)nativePtr)->GetEulerAngles()); }
			void set(Vector3 value) { ((thomas::object::component::Transform*)nativePtr)->SetRotation(value.y, value.x, value.z); }
		}
		
		property Vector3 scale
		{
			Vector3 get() { return Vector3(((thomas::object::component::Transform*)nativePtr)->GetScale()); }
			void set(Vector3 value) { ((thomas::object::component::Transform*)nativePtr)->SetScale(thomas::math::Vector3(value.x, value.y, value.z)); }
		}
		[BrowsableAttribute(false)]
		property Vector3 localScale
		{
			Vector3 get() { return Vector3(((thomas::object::component::Transform*)nativePtr)->m_localScale); }
			void set(Vector3 value) { ((thomas::object::component::Transform*)nativePtr)->m_localScale = thomas::math::Vector3(value.x, value.y, value.z); }
		}
	
		void Update() override
		{
			if (((thomas::object::component::Transform*)nativePtr)->IsDirty())
			{
				((thomas::object::component::Transform*)nativePtr)->SetDirty(false);
				OnPropertyChanged("position");
				OnPropertyChanged("eulerAngles");
				OnPropertyChanged("scale");
			}
		}
		
	};
}