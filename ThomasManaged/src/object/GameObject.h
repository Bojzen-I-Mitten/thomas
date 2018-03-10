#pragma once
#pragma unmanaged
#include <thomas\object\GameObject.h>
#include <thomas\editor\EditorCamera.h>
#pragma managed
#include <string>
#include <msclr\marshal_cppstd.h>

#include "../Scene.h"
#include "Object.h"
#include "Component.h"
#include "component\Transform.h"
#include "../attributes/CustomAttributes.h"
#using "PresentationFramework.dll"
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Collections::ObjectModel;
using namespace System::Threading;
namespace ThomasEditor {

	public ref class GameObject : public Object
	{
		System::Object^ m_componentsLock = gcnew System::Object();
		ObservableCollection<Component^> m_components;
		Transform^ m_transform;
		Scene^ scene;

		

		GameObject() : Object(new thomas::object::GameObject("gameobject")) {
			s_lastObject = this;
			m_name = "gameobject";
			System::Windows::Data::BindingOperations::EnableCollectionSynchronization(%m_components, m_componentsLock);
		}

	internal:
		void PostLoad()
		{

			for (int i = 0; i < m_components.Count; i++)
			{
				Component^ component = m_components[i];

				Type^ typ = component->GetType();
				if (typ->IsDefined(ExecuteInEditor::typeid, false)) {
					component->Awake();
				}
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
			for (int i = 0; i < m_components.Count; i++)
			{
				Component^ component = m_components[i];
				if (component->IsAwake())
					component->Update();
			}
		}

		void RenderGizmos()
		{
			for (int i = 0; i < m_components.Count; i++)
			{
				m_components[i]->OnDrawGizmos();
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
		static GameObject^ s_lastObject;

		GameObject(String^ name) : Object(new thomas::object::GameObject(msclr::interop::marshal_as<std::string>(name))) {
			m_name = name;
			m_transform = AddComponent<Transform^>();
			((thomas::object::GameObject*)nativePtr)->m_transform = (thomas::object::component::Transform*)m_transform->nativePtr;
			Scene::CurrentScene->GameObjects->Add(this);
			scene = Scene::CurrentScene;
			System::Windows::Data::BindingOperations::EnableCollectionSynchronization(%m_components, m_componentsLock);
			
		}

		virtual void Destroy() override
		{
			for (int i = 0; i < m_components.Count; i++) {
				m_components[i]->Destroy();
				i--;
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
			void set(Transform^ value) {
				if (value)
				{
					m_transform = value;
					((thomas::object::GameObject*)nativePtr)->m_transform = (thomas::object::component::Transform*)m_transform->nativePtr;
				}
				
			}
		}

		property ObservableCollection<Component^>^ Components {
			ObservableCollection<Component^>^ get() {
				return %m_components;
			}
		}



		generic<typename T>
		where T : Component
		T AddComponent() {
			
			T component = (T)Activator::CreateInstance(T::typeid);
			((Component^)component)->setGameObject(this);
			m_components.Add((Component^)component);
			Type^ typ = T::typeid;
			if (typ->IsDefined(ExecuteInEditor::typeid, false) || scene->IsPlaying())
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
			for each(GameObject^ gameObject in Scene::CurrentScene->GameObjects)
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