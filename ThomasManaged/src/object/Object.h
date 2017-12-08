#pragma once
#pragma unmanaged
#include <thomas\object\Object.h>
#pragma managed
#include <string>
#include <msclr\marshal_cppstd.h>

using namespace System;
using namespace System::Collections::Generic;
namespace ThomasEditor {
	public ref class Object
	{
		static List<Object^> s_objects;

	internal:
		thomas::object::Object* nativePtr;
	public:

		virtual void OnDestroy() { nativePtr->OnDestroy(); }

		static void Destroy(Object^ object) { object->Destroy(); }
		
		Object()
		{
			s_objects.Add(this);
		}

		virtual void Destroy()
		{
			nativePtr->Destroy(nativePtr);
			s_objects.Remove(this);
		}

		static List<Object^>^ GetObjects()
		{
			return %s_objects;
		}
	};
}