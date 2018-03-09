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
#include "../attributes/CustomAttributes.h"
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Collections::ObjectModel;
namespace ThomasEditor {

	public ref class GameObject : public Object
	{
		
		ObservableCollection<Component^> m_components;
		Transform^ m_transform;
		static ObservableCollection<GameObject^> s_gameObjects;
		
	internal:
		
		static bool s_playing;
		static void Play()
		{
			s_playing = true;
			for each(GameObject^ gObj in s_gameObjects)
			{
				gObj->Awake();
			}
		}
		void Awake()
		{
			for each(Component^ component in m_components)
			{
				if (!component->IsAwake())
					component->Awake();
			}
		}
		void UpdateComponents()
		{
			for each(Component^ component in m_components)
			{
				if(component->IsAwake())
					component->Update();
			}
				
		}

		void RenderGizmos()
		{
			for each(Component^ component in m_components)
			{
				component->OnDrawGizmos();
			}
		}

		void RenderSelectedGizmos()
		{
			for each(Component^ component in m_components)
			{
				component->OnDrawGizmosSelected();
			}
		}

	public:
		GameObject(String^ name) : Object(new thomas::object::GameObject(msclr::interop::marshal_as<std::string>(name))) {
			m_name = name;
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
			thomas::object::Object::Destroy(nativePtr);
			m_components.Clear();
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
			Type^ typ = T::typeid;
			if (typ->IsDefined(ExecuteInEditor::typeid, false) || s_playing)
			{
				component->Awake();
			}
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
	};
}