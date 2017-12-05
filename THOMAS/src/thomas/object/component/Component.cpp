#include "Component.h"
#include "../GameObject.h"
namespace thomas
{
	namespace object
	{
		namespace component
		{
			bool Component::GetActive()
			{
				return m_gameObject->GetActive() && m_active;
			}

			
		}
	}
}