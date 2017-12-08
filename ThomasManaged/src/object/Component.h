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
		GameObject^ m_gameObject;
		void setGameObject(GameObject^ gObj);
		virtual void Update() {};
	public:

		[BrowsableAttribute(false)]
		property GameObject^ gameObject
		{
			GameObject^ get() { return m_gameObject; }
		}


	};
}