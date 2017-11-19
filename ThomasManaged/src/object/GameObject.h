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
namespace ThomasEditor {

	public ref class GameObject : public Object
	{
		thomas::object::GameObject* nativePtr;
		List<Component^> components;
		Transform^ m_transform;
	internal:
		GameObject(thomas::object::GameObject* ptr) : Object((thomas::object::Object*)ptr) {
			nativePtr = ptr;
			m_transform = gcnew Transform(nativePtr->GetComponent<thomas::object::component::Transform>());
		}
	public:
		GameObject(String^ type) : Object(type) {
			nativePtr = new thomas::object::GameObject(msclr::interop::marshal_as<std::string>(type));
			m_transform = gcnew Transform(nativePtr->GetComponent<thomas::object::component::Transform>());
		}

		property Transform^ transform {
			Transform^ get() {
				return m_transform;
			}
		}

		generic<typename T>
		where T : Component
		T AddComponentWithType() {
			
			T component = (T)Activator::CreateInstance(T::typeid);
			components.Add((Component^)component);
			return component;
		}

		generic<typename T>
		where T : Component
		T GetComponent()
		{
			for each(Component^ component in components) {
				if (T comp = safe_cast<T>(component))
					return comp;
			}
			return T();
		}
		
		/*

		generic<typename T>
		T GetComponent() { return nativePtr->GetComponent<T>(); }
		generic<typename T>
		List<T> GetComponents() { return nativePtr->GetComponents<T>(); }
		*/
		static GameObject^ Find(String^ type) {
			thomas::object::GameObject* nativeGameObject = thomas::object::GameObject::Find(msclr::interop::marshal_as<std::string>(type));
			if (nativeGameObject != nullptr)
				return gcnew GameObject(nativeGameObject);
			else
				return nullptr;
		};


		/*
		generic<typename T>
		static List<GameObject^> FindGameObjectsWithComponent()
		{
			std::vector<thomas::object::GameObject*> nativeGameObjects = thomas::object::GameObject::FindGameObjectsWithComponent<T>();

			List<GameObject^>^ managedGameObjects = gcnew List<GameObject^>(nativeGameObjects.size());

			for (thomas::object::GameObject* nativeGameObject : nativeGameObjects)
			{
				managedGameObjects->Add(gcnew GameObject(nativeGameObject));
			}
			return managedGameObjects;
		};
		*/


		static List<GameObject^>^ GetGameObjects()
		{
			std::vector<thomas::object::GameObject*> nativeGameObjects = thomas::object::GameObject::GetGameObjects();

			List<GameObject^>^ managedGameObjects = gcnew List<GameObject^>(nativeGameObjects.size());

			for (thomas::object::GameObject* nativeGameObject : nativeGameObjects)
			{
				managedGameObjects->Add(gcnew GameObject(nativeGameObject));
			}
			return managedGameObjects;
		}

	};
}