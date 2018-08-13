#pragma once
#pragma unmanaged
#include <thomas\object\GameObject.h>
#include <thomas\editor\EditorCamera.h>
#include <..\..\ThomasEngine\src\resource\Resources.h>

#pragma managed
#include <string>
#include <msclr\marshal_cppstd.h>
#include "../Scene.h"
#include "Object.h"
#include "Component.h"
#include "component\Transform.h"
#include "component\ScriptComponent.h"
#include "../attributes/CustomAttributes.h"
#include "component\RenderComponent.h"
#using "PresentationFramework.dll"
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Collections::ObjectModel;
using namespace System::Threading;
using namespace System::Linq;

namespace ThomasEditor 
{
	public ref class GameObject : public Object
	{
		ObservableCollection<Component^> m_components;
		Transform^ m_transform;
		Scene^ scene;

		GameObject() : Object(new thomas::object::GameObject("gameobject")) 
		{
			s_lastObject = this;
			m_name = "gameobject";

			System::Windows::Data::BindingOperations::EnableCollectionSynchronization(%m_components, m_componentsLock);
		}

	internal:
		System::Object^ m_componentsLock = gcnew System::Object();

		void PostLoad()
		{
			scene = Scene::CurrentScene;
			m_transform = GetComponent<Transform^>();
			
			List<Component^>^ editorComponents = gcnew List<Component^>;
			for (int i = 0; i < m_components.Count; i++)
			{
				Component^ component = m_components[i];

				Type^ typ = component->GetType();
				if (typ->IsDefined(ExecuteInEditor::typeid, false) && component->enabled && !component->initialized) {
					editorComponents->Add(component);
				}
			}
			
			initComponents(editorComponents);	
		}

		void initComponents(List<Component^>^ components)
		{
			for each(Component^ component in components)
			{
				component->Awake();
				component->initialized = true;
			}

			for each(Component^ component in components)
			{
				component->OnEnable();
			}

			for each(Component^ component in components)
			{
				component->Start();
			}
		}
		
		void Update()
		{
			Monitor::Enter(m_componentsLock);
			if (Scene::CurrentScene->IsPlaying())
			{
				List<Component^>^ uninitializedComponents = gcnew List<Component^>;
				for each(Component^ component in m_components)
				{
					if (!component->initialized)
						uninitializedComponents->Add(component);

				}
				initComponents(uninitializedComponents);
			}


			for (int i = 0; i < m_components.Count; i++)
			{
				Component^ component = m_components[i];
				if (component->initialized && component->enabled)
					component->Update();
			}
			Monitor::Exit(m_componentsLock);
		}

		void FixedUpdate()
		{
			Monitor::Enter(m_componentsLock);
			for (int i = 0; i < m_components.Count; i++)
			{
				Component^ component = m_components[i];
				if (component->initialized && component->enabled)
					component->FixedUpdate();
			}
			Monitor::Exit(m_componentsLock);
		}

		void RenderGizmos()
		{
			Monitor::Enter(m_componentsLock);
			for (int i = 0; i < m_components.Count; i++)
			{
				m_components[i]->OnDrawGizmos();
			}
			Monitor::Exit(m_componentsLock);
		}

		void RenderSelectedGizmos()
		{
			Monitor::Enter(m_componentsLock);
			for each(Component^ component in m_components)
			{
				component->OnDrawGizmosSelected();
			}
			Monitor::Exit(m_componentsLock);
		}

	public:
		static GameObject^ s_lastObject;

		GameObject(String^ name) : Object(new thomas::object::GameObject(Utility::ConvertString(name))) 
		{
			s_lastObject = this;
			m_name = name;
			m_transform = AddComponent<Transform^>();
			((thomas::object::GameObject*)nativePtr)->m_transform = (thomas::object::component::Transform*)m_transform->nativePtr;

			Monitor::Enter(Scene::CurrentScene->GetGameObjectsLock());

			Scene::CurrentScene->GameObjects->Add(this);
			scene = Scene::CurrentScene;
			System::Windows::Data::BindingOperations::EnableCollectionSynchronization(%m_components, m_componentsLock);
			
			Monitor::Exit(Scene::CurrentScene->GetGameObjectsLock());
		}

		virtual void Destroy() override;

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
		[Xml::Serialization::XmlIgnoreAttribute]
		property Transform^ transform 
		{
			Transform^ get() 
			{
				return m_transform;
			}

			void set(Transform^ value) 
			{
				if (value)
				{
					m_transform = value;
					((thomas::object::GameObject*)nativePtr)->m_transform = (thomas::object::component::Transform*)m_transform->nativePtr;
				}
			}
		}

		property ObservableCollection<Component^>^ Components 
		{
			ObservableCollection<Component^>^ get() 
			{
				return %m_components;
			}
		}

		generic<typename T>
		where T : Component
		T AddComponent() 
		{
			Monitor::Enter(m_componentsLock);
			Type^ typ = T::typeid;

			T existingComponent = GetComponent<T>();
			if (existingComponent && typ->IsDefined(DisallowMultipleComponent::typeid, false))
			{	
				//LOG("Cannot add multiple instances of " << typ->Name);
				return T();
			}

			T component = (T)Activator::CreateInstance(T::typeid);
			((Component^)component)->setGameObject(this);
			m_components.Add((Component^)component);
			
			if ((typ->IsDefined(ExecuteInEditor::typeid, false) || scene->IsPlaying()) && !component->initialized && component->enabled)
			{
				component->Awake();
				component->initialized = true;
				component->OnEnable();
				component->Start();
			}

			Monitor::Exit(m_componentsLock);
			return component;
		}

		generic<typename T>
		where T : Component
		T GetComponent()
		{

			List<T> tComponents = Enumerable::OfType<T>(%m_components);
			if (tComponents.Count > 0)
				return tComponents[0];
			else
				return T();
		}

		generic<typename T>
		where T : Component
		List<T>^ GetComponents()
		{
			List<T>^ list = gcnew List<T>(Enumerable::OfType<T>(%m_components));
			return list;
		}


		bool HasComponentOfType(Type^ T)
		{
			for each(Component^ comp in m_components) {
				if (comp->GetType() == T)
					return true;
			}
			return false;
		}

		//generic<typename T>
		//where T : Component
		//List<T>^ GetComponents()
		//{
		//	return gcnew List<T>(Enumerable::OfType<T>(%m_components));
		//}
		//

		static GameObject^ Find(String^ name) 
		{
			for each(GameObject^ gameObject in Scene::CurrentScene->GameObjects)
			{
				if (gameObject->Name == name)
					return gameObject;
			}
			return nullptr;
		}

		static GameObject^ CreatePrimitive(PrimitiveType type);

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