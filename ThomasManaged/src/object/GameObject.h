#pragma once
#pragma unmanaged
#include <thomas\object\GameObject.h>
#pragma managed
#include <string>
#include <msclr\marshal_cppstd.h>

#include "Object.h"
#include "Component.h"
#include "component\Transform.h"
using namespace System;
using namespace System::Collections::Generic;
using namespace System::ComponentModel;
using namespace System::Collections::ObjectModel;
using namespace Mindscape::WpfElements::WpfPropertyGrid;
namespace ThomasEditor {

	public ref class GameObject : public Object, public INotifyPropertyChanged
	{
		
		ObservableDictionary<String^,Component^> m_components;
		Transform^ m_transform;
		static ObservableCollection<GameObject^> s_gameObjects;
		String^ m_name;
	public:
		GameObject(String^ name) {
			m_name = name;
			nativePtr = new thomas::object::GameObject(msclr::interop::marshal_as<std::string>(name));
			m_transform = AddComponent<Transform^>();
			s_gameObjects.Add(this);
			
		}

		virtual event PropertyChangedEventHandler^ PropertyChanged;

		property String^ Name
		{
			String^ get() { return m_name; }

			void set(String^ value)
			{
				m_name = value;
				OnPropertyChanged("Name");
			}
		}

		property bool activeSelf
		{
			bool get()
			{
				return ((thomas::object::GameObject*)nativePtr)->m_activeSelf;
			}
			void set(bool value)
			{
				((thomas::object::GameObject*)nativePtr)->m_activeSelf = value;
			}
		}
		

		String^ ToString() override
		{
			return Name;
		}

		[BrowsableAttribute(false)]
		property Transform^ transform {
			Transform^ get() {
				return m_transform;
			}
		}

		property ObservableDictionary<String^, Component^>^ Components {
			ObservableDictionary<String^, Component^>^ get() {
				return %m_components;
			}
		}

		static property ObservableCollection<GameObject^>^ GameObjects {
			ObservableCollection<GameObject^>^ get() {
				return %s_gameObjects;
			}
		}

		generic<typename T>
		where T : Component
		T AddComponent() {
			
			T component = (T)Activator::CreateInstance(T::typeid);
			((Component^)component)->setGameObject(this);
			m_components.Add((T::typeid)->Name, (Component^)component);
			return component;
		}

		generic<typename T>
		where T : Component
		T GetComponent()
		{
			if (m_components.ContainsKey((T::typeid)->Name))
				return (T)m_components[(T::typeid)->Name];
			else
				return T();
		}
		

		static GameObject^ Find(String^ name) {
			for each(GameObject^ gameObject in s_gameObjects)
			{
				if (gameObject->Name == name)
					return gameObject;
			}
			return nullptr;
		};


		void OnPropertyChanged(String^ info)
		{
			PropertyChanged(this, gcnew PropertyChangedEventArgs(info));
		}

	};
}