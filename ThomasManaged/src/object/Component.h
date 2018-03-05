#pragma once
#pragma unmanaged
#include <thomas\object\component\Component.h>
#pragma managed
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
	internal:
		Component(thomas::object::component::Component* ptr) : Object(ptr) {};
		GameObject^ m_gameObject;
		void setGameObject(GameObject^ gObj);
		virtual void Update() {};
		virtual void OnDrawGizmosSelected() { ((thomas::object::component::Component*)nativePtr)->OnDrawGizmosSelected(); }
		virtual void OnDrawGizmos() { ((thomas::object::component::Component*)nativePtr)->OnDrawGizmos(); }

	private:
		bool m_awake = false;
	public:
		bool IsAwake() { return m_awake; }
		void Awake() { m_awake = true; }

		[BrowsableAttribute(false)]
		property GameObject^ gameObject
		{
			GameObject^ get() { return m_gameObject; }
		}


	};
}