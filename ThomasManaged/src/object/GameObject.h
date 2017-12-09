#pragma once
#pragma unmanaged
#include <thomas\object\GameObject.h>
#include <thomas\editor\EditorCamera.h>
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
namespace ThomasEditor {

	public ref class GameObject : public Object, public INotifyPropertyChanged
	{
		
		ObservableCollection<Component^> m_components;
		Transform^ m_transform;
		static ObservableCollection<GameObject^> s_gameObjects;
		String^ m_name;

	internal:
		void UpdateComponents()
		{
			((thomas::object::GameObject*)nativePtr)->UpdateComponents();
		}
	public:
		GameObject(String^ name) {
			m_name = name;
			nativePtr = new thomas::object::GameObject(msclr::interop::marshal_as<std::string>(name));
			m_transform = AddComponent<Transform^>();
			((thomas::object::GameObject*)nativePtr)->m_transform = (thomas::object::component::Transform*)m_transform->nativePtr;
			s_gameObjects.Add(this);
			
		}

		virtual void Destroy() override
		{
			for each(Component^ component in m_components)
			{
				component->Destroy();
			}
			m_components.Clear();
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

		property ObservableCollection<Component^>^ Components {
			ObservableCollection<Component^>^ get() {
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
			m_components.Add((Component^)component);
			return component;
		}

		generic<typename T>
		where T : Component
		T GetComponent()
		{
			for each(Component^ component in m_components)
			{
				if (T::typeid == component::typeid)
					return (T)component;
			}
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


		bool GetActive()
		{
			return ((thomas::object::GameObject*)nativePtr)->GetActive();
		}

		void SetActive(bool active)
		{
			((thomas::object::GameObject*)nativePtr)->SetActive(active);
		}

		
		void OnPropertyChanged(String^ info)
		{
			PropertyChanged(this, gcnew PropertyChangedEventArgs(info));
		}

		void OnSelection()
		{
			thomas::editor::EditorCamera::SelectObject((thomas::object::GameObject*)nativePtr);
		}

	};
}