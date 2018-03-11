#include "Component.h"
#include "../GameObject.h"
namespace thomas
{
	namespace object
	{
		namespace component
		{
			void Component::Destroy()
			{
				for (unsigned int i = 0; i < m_gameObject->m_components.size(); i++)
				{
					if (m_gameObject->m_components[i] == this)
					{
						m_gameObject->m_components.erase(m_gameObject->m_components.begin() + i);
					}
				}
				Object::Destroy(this);
			}
		}
	}
}