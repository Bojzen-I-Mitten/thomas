#include "Component.h"
#include "../GameObject.h"
namespace thomas
{
	namespace object
	{
		namespace component
		{

			Component::Component(std::string name) : Object(name)
			{
			}

			bool Component::GetActive()
			{
				return m_gameObject->GetActive() && m_active;
			}

			
		}
	}
}