#include "GameObject.h"
namespace thomas
{
	namespace object
	{
		std::vector<GameObject*> GameObject::s_gameObjects;


		GameObject::GameObject(std::string name)
		{
			m_activeSelf = true;
			m_name = name;
			m_selected = false;
		}
		GameObject::~GameObject()
		{
			for (unsigned int i = 0; i < this->m_components.size(); i++)
			{
				if (this->m_components[i])
				{
					Object::Destroy(this->m_components[i]);
				}     
					
			}
			this->m_components.clear();

			for (unsigned int i = 0; i < s_gameObjects.size(); i++)
			{
				if (s_gameObjects[i] == this)
				{
					s_gameObjects.erase(s_gameObjects.begin() + i);
				}
			}

			Object::Destroy(this);
		}
		GameObject * GameObject::Find(std::string type)
		{
			for (unsigned int i = 0; i < s_gameObjects.size(); i++)
			{
				if (s_gameObjects[i]->m_name == type)
					return s_gameObjects[i];
			}
			return NULL;
		}

		std::vector<GameObject*> GameObject::GetGameObjects()
		{
			return s_gameObjects;
		}

		bool GameObject::Destroy(GameObject * gameObject)
		{

			for (unsigned int i = 0; i < gameObject->m_components.size(); i++)
			{
				if (gameObject->m_components[i])
				{
					Object::Destroy(gameObject->m_components[i]);
					
				}

			}
			gameObject->m_components.clear();

			for (unsigned int i = 0; i < s_gameObjects.size(); i++)
			{
				if (s_gameObjects[i] == gameObject)
				{
					s_gameObjects.erase(s_gameObjects.begin() + i);

				}
			}

			Object::Destroy(gameObject);

			return true;
		}
		void GameObject::Destroy()
		{
			for (unsigned int i = 0; i < s_gameObjects.size(); ++i)
			{
				Destroy(s_gameObjects[i]);
			}
			s_gameObjects.clear();
		}
		bool GameObject::GetActive()
		{
			if (m_transform && m_transform->GetParent())
			{
				return m_activeSelf && m_transform->GetParent()->m_gameObject->m_activeSelf;
			}
			return m_activeSelf;
		}

		void GameObject::SetActive(bool active)
		{
			m_activeSelf = active;
			if (m_transform && m_transform->GetParent())
			{
				return m_transform->GetParent()->m_gameObject->SetActive(active);
			}
		}

		void GameObject::SetSelection(bool selected)
		{
			m_selected = selected;
		}

		bool GameObject::GetSelection()
		{
			return m_selected;
		}

	}
}