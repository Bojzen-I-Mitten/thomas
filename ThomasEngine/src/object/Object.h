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
	private:
		
	protected:
		
		String^ m_name;
	internal:
		[System::Runtime::Serialization::DataMemberAttribute]
		Guid m_guid;

		[NonSerializedAttribute]
		thomas::object::Object* nativePtr;

		static Object^ Find(Guid guid)
		{
			for each(Object^ o in s_objects)
			{
				if (o->m_guid == guid)
					return o;
			}
			return nullptr;
		}

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
			m_guid = Guid::NewGuid();
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

		static bool operator ==(Object^ a, Object^ b)
		{
			if (Object::ReferenceEquals(nullptr, a))
				return Object::ReferenceEquals(nullptr, b);

			if (Object::ReferenceEquals(nullptr, b))
				return false;

			return a->nativePtr == b->nativePtr;
		}

		static bool operator !=(Object^ a, Object^ b)
		{

			if (Object::ReferenceEquals(nullptr, a))
				return !Object::ReferenceEquals(nullptr, b);

			if (Object::ReferenceEquals(nullptr, b))
				return true;

			return a->nativePtr != b->nativePtr;
			
		}

		static operator bool(Object^ object)
		{
			return object != nullptr;
		}
	};
}