#pragma once
#pragma unmanaged
#include <thomas\object\component\Component.h>
#pragma managed
#include <string>
#include <msclr\marshal_cppstd.h>
#include "Object.h"
using namespace System;
using namespace System::Collections::Generic;
namespace ThomasEditor {

	ref class GameObject;

	public ref class Component : public Object
	{
		thomas::object::component::Component* nativePtr;
	internal:
		Component(thomas::object::component::Component* ptr) : Object((thomas::object::Object*)ptr) {
			nativePtr = ptr;
		}
	public:
		Component(String^ type) : Object(type) {
			nativePtr = new thomas::object::component::Component(msclr::interop::marshal_as<std::string>(type));
		}

		property GameObject^ gameObject
		{
			GameObject^ get();
		}


	};
}