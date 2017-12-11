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
		
		Object(thomas::object::Object* ptr)
		{
			nativePtr = ptr;
			s_objects.Add(this);
			thomas::object::Object::Add(ptr);
		}

		virtual void Destroy()
		{
			nativePtr->Destroy(nativePtr);
			s_objects.Remove(this);
		}

		static Object^ GetObject(thomas::object::Object* ptr)
		{
			for each(Object^ object in s_objects)
			{
				if (object->nativePtr == ptr)
					return object;
			}
			return nullptr;
		}

		static List<Object^>^ GetObjects()
		{
			return %s_objects;
		}
	};
}