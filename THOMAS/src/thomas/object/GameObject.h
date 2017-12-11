#pragma once
#include "Object.h"
#include "component\Components.h"
#include <vector>
namespace thomas
{
	class Scene;
	namespace object
	{



		class THOMAS_API GameObject : public Object
		{
		private:
			
		public:
			std::vector<component::Component*> m_components;
			component::Transform* m_transform;
			bool m_activeSelf;

			GameObject(std::string type);
			~GameObject();

			static GameObject* Find(std::string type);

			template<typename T>
			static std::vector<GameObject*> FindGameObjectsWithComponent();
			static std::vector<GameObject*> GetGameObjects();

			template<typename T>
			T* GetComponent();

			static bool Destroy(GameObject *object);
			static void Destroy();
			template<typename T>
			static T* Instantiate(component::Transform* parent, Scene* scene);
			template<typename T>
			static T* Instantiate(math::Vector3 position, math::Quaternion rotation, Scene* scene);
			template<typename T>
			static T* Instantiate(math::Vector3 position, math::Quaternion rotation, component::Transform* parent, Scene* scene);
			bool GetActive();
			void SetActive(bool active);
			void SetSelection(bool selected);
			bool GetSelection();

		private:
			bool m_selected;
			static std::vector<GameObject*> s_gameObjects;
		};
		

		template<typename T>
		std::vector<GameObject*> GameObject::FindGameObjectsWithComponent()
		{
			std::vector<GameObject*> gameObjectsWithComponent;
			for (UINT i = 0; i < s_gameObjects.size(); i++)
			{
				T* component = s_gameObjects[i]->GetComponent<T>();
				if (component)
					gameObjectsWithComponent.push_back(s_gameObjects[i]);
			}
			return gameObjectsWithComponent;
		}

		template<typename T>
		T* GameObject::GetComponent()
		{

			for (UINT i = 0; i < m_components.size(); i++)
			{
				T* comp = dynamic_cast<T*>(m_components[i]);
				if (comp)
					return comp;
			}
			return NULL;
		}

		template<typename T>
		T * GameObject::Instantiate(component::Transform * parent, Scene * scene)
		{
			T* gameObject = Object::Instantiate<T>(scene);
			s_gameObjects.push_back(gameObject);
			gameObject->m_transform = gameObject->AddComponent<component::Transform>();
			gameObject->m_transform->SetParent(parent);
			gameObject->SetActive(true);
			gameObject->Start();
			return gameObject;
		}

		template<typename T>
		T * GameObject::Instantiate(math::Vector3 position, math::Quaternion rotation, Scene * scene)
		{
			T* gameObject = Object::Instantiate<T>(scene);
			s_gameObjects.push_back(gameObject);
			gameObject->m_transform = gameObject->AddComponent<component::Transform>();
			gameObject->m_transform->SetRotation(rotation);
			gameObject->m_transform->SetPosition(position);
			gameObject->SetActive(true);
			gameObject->Start();
			return gameObject;
		}

		template<typename T>
		T * GameObject::Instantiate(math::Vector3 position, math::Quaternion rotation, component::Transform * parent, Scene * scene)
		{
			T* gameObject = Object::Instantiate<T>(scene);
			s_gameObjects.push_back(gameObject);
			gameObject->m_transform = gameObject->AddComponent<component::Transform>();
			gameObject->m_transform->SetParent(parent);
			gameObject->m_transform->SetRotation(rotation);
			gameObject->m_transform->SetPosition(position);
			gameObject->SetActive(parent->m_gameObject->GetActive());
			gameObject->Start();
			return gameObject;
		}
	}
}