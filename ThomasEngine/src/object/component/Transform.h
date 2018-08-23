#pragma once
#pragma unmanaged
#include <thomas\object\component\Transform.h>
#pragma managed

#include "../../math/Math.h"
#include "../Component.h"
#include <string>
#include <msclr\marshal_cppstd.h>

using namespace System::ComponentModel;
namespace ThomasEngine
{
	[DisallowMultipleComponent]
	[ExecuteInEditor]
	[HideInInspector]
	public ref class Transform : public Component
	{
	public:
		Transform() : Component(new thomas::object::component::Transform()) 
		{
		}

		[BrowsableAttribute(false)]
		property Transform^ parent 
		{
			Transform^ get() 
			{
				if (((thomas::object::component::Transform*)nativePtr)->GetParent())
					return (Transform^)GetObject(((thomas::object::component::Transform*)nativePtr)->GetParent());
				else
					return nullptr;
				
			}

			void set(Transform^ value);
		}

		[BrowsableAttribute(false)]
		property List<Transform^>^ children 
		{
			List<Transform^>^ get() 
			{
				std::vector<thomas::object::component::Transform*> nativeChildren = ((thomas::object::component::Transform*)nativePtr)->GetChildren();
				List<Transform^>^ managedChildren = gcnew List<Transform^>(nativeChildren.size());

				for (thomas::object::component::Transform* nativeChild : nativeChildren)
					managedChildren->Add((Transform^)GetObject(nativeChild));

				return managedChildren;
			}
		}

		[BrowsableAttribute(false)]
		property Vector3 position
		{
			Vector3 get() { return Vector3(((thomas::object::component::Transform*)nativePtr)->GetPosition()); }
			void set(Vector3 value) { ((thomas::object::component::Transform*)nativePtr)->SetPosition(thomas::math::Vector3(value.x, value.y, value.z)); OnPropertyChanged("localPosition");}
		}

		[DisplayNameAttribute("position")]
		property Vector3 localPosition
		{
			Vector3 get() { return Vector3(((thomas::object::component::Transform*)nativePtr)->GetLocalPosition()); }
			void set(Vector3 value) { ((thomas::object::component::Transform*)nativePtr)->SetLocalPosition(value.x, value.y, value.z); }
		}

		[BrowsableAttribute(false)]
		property Quaternion rotation
		{
			Quaternion get() { return Quaternion(((thomas::object::component::Transform*)nativePtr)->GetRotation()); }
			void set(Quaternion value) { ((thomas::object::component::Transform*)nativePtr)->SetRotation(thomas::math::Quaternion(value.x, value.y, value.z, value.w)); OnPropertyChanged("localEulerAngles");}
		}
	
		[BrowsableAttribute(false)]
		property Vector3 eulerAngles
		{
			Vector3 get() { return Vector3(((thomas::object::component::Transform*)nativePtr)->GetEulerAngles()); }
			void set(Vector3 value) { ((thomas::object::component::Transform*)nativePtr)->SetRotation(value.y, value.x, value.z); OnPropertyChanged("localEulerAngles");}
		}

		[DisplayNameAttribute("rotation")]
		property Vector3 localEulerAngles
		{
			Vector3 get() { return Vector3(((thomas::object::component::Transform*)nativePtr)->GetLocalEulerAngles()); }
			void set(Vector3 value) { ((thomas::object::component::Transform*)nativePtr)->SetLocalRotation(value.y, value.x, value.z); }
		}
		
		[BrowsableAttribute(false)]
		property Vector3 scale
		{
			Vector3 get() { return Vector3(((thomas::object::component::Transform*)nativePtr)->GetScale()); }
			void set(Vector3 value) { ((thomas::object::component::Transform*)nativePtr)->SetScale(thomas::math::Vector3(value.x, value.y, value.z)); OnPropertyChanged("localScale");}
		}

		[DisplayNameAttribute("scale")]
		property Vector3 localScale
		{
			Vector3 get() { return Vector3(((thomas::object::component::Transform*)nativePtr)->GetLocalScale()); }
			void set(Vector3 value) { ((thomas::object::component::Transform*)nativePtr)->SetLocalScale(thomas::math::Vector3(value.x, value.y, value.z)); }
		}

		bool IsChildOf(Transform^ _parent)
		{
			if (this->nativePtr == _parent->nativePtr)
				return true;
			else if (parent != nullptr)
				return parent->IsChildOf(_parent);
			else
				return false;
		}

		void OnDestroy() override;
		void Update() override
		{
			if (((thomas::object::component::Transform*)nativePtr)->IsDirty())
			{
				((thomas::object::component::Transform*)nativePtr)->SetDirty(false);
				OnPropertyChanged("localPosition");
				OnPropertyChanged("localEulerAngles");
				OnPropertyChanged("localScale");
			}
		}	
	};
}