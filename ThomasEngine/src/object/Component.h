#pragma once
#pragma unmanaged
#include <thomas\object\component\Component.h>
#pragma managed
#include "../attributes/CustomAttributes.h"
#include <string>
#include <msclr\marshal_cppstd.h>
#include "Object.h"
using namespace System;
using namespace System::Collections::Generic;
using namespace System::ComponentModel;

namespace ThomasEditor {



	ref class GameObject;
	public ref class Component : public Object
	{
		Component();
	internal:
		Component(thomas::object::component::Component* ptr);
		
		void setGameObject(GameObject^ gObj);

		virtual void Awake() { ((thomas::object::component::Component*)nativePtr)->Awake(); }
		virtual void Start() {};
		virtual void OnEnable() { ((thomas::object::component::Component*)nativePtr)->OnEnable(); }
		virtual void OnDisable() { ((thomas::object::component::Component*)nativePtr)->OnDisable(); };
		virtual void Update() { ((thomas::object::component::Component*)nativePtr)->Update(); }
		virtual void OnDrawGizmosSelected() { ((thomas::object::component::Component*)nativePtr)->OnDrawGizmosSelected(); }
		virtual void OnDrawGizmos() { ((thomas::object::component::Component*)nativePtr)->OnDrawGizmos(); }

		GameObject^ m_gameObject;
		
		property bool initialized
		{
			bool get() { return ((thomas::object::component::Component*)nativePtr)->initialized; }
			void set(bool value) { ((thomas::object::component::Component*)nativePtr)->initialized = value; }
		}

	public:
		bool enabled = true;

		[BrowsableAttribute(false)]
		property GameObject^ gameObject
		{
			GameObject^ get() { return m_gameObject; }
		}

		virtual void Destroy() override;

		static List<Type^>^ GetAllComponentTypes();
		static List<Type^>^ GetAllAddableComponentTypes();
	};
}