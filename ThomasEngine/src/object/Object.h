#pragma once
#pragma unmanaged
#include <thomas\object\Object.h>
#pragma managed
#include <string>
#include <msclr\marshal_cppstd.h>

using namespace System;
using namespace System::Collections::Generic;
using namespace System::ComponentModel;
namespace ThomasEditor {

	[SerializableAttribute]
	public ref class Object: public INotifyPropertyChanged
	{
		static List<Object^> s_objects;
	protected:
		String^ m_name;
	internal:
		[NonSerializedAttribute]
		thomas::object::Object* nativePtr;
	public:
		[field:NonSerializedAttribute]
		virtual event PropertyChangedEventHandler^ PropertyChanged;
		virtual void OnDestroy() {nativePtr->OnDestroy(); }

		static void Destroy(Object^ object) { object->Destroy(); }
		
		void OnPropertyChanged(String^ info)
		{
			PropertyChanged(this, gcnew PropertyChangedEventArgs(info));
		}

		[BrowsableAttribute(false)]
		property String^ Name
		{
			String^ get() { return m_name; }

			void set(String^ value)
			{
				m_name = value;
				OnPropertyChanged("Name");
			}
		}

		Object(thomas::object::Object* ptr)
		{
			nativePtr = ptr;
			s_objects.Add(this);
			thomas::object::Object::Add(ptr);
		}

		virtual void Destroy()
		{
			thomas::object::Object::Destroy(nativePtr);
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

		static operator bool(Object^ object)
		{
			return object != nullptr;
		}
	};
}