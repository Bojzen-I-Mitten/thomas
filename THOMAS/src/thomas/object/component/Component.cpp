#include "Component.h"

#include "../GameObject.h"
thomas::object::component::Component::~Component()
{
	for (int i = 0; i < m_gameObject->m_components.size(); i++)
	{
		Component* component = m_gameObject->m_components[i];
		if (component == this)
		{
			m_gameObject->m_components.erase(m_gameObject->m_components.begin() + i);
		}
	}
}
