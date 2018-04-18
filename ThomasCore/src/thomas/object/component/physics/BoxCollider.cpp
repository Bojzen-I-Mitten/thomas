#include "BoxCollider.h"

namespace thomas
{
	namespace object
	{
		namespace component
		{
			BoxCollider::BoxCollider() : Collider(new btBoxShape(btVector3(1,1,1)))
			{
				m_center = math::Vector3(0, 0, 0);
				m_size = math::Vector3(1, 1, 1);
			}

			void BoxCollider::SetCenter(math::Vector3 center)
			{
				m_center = center;
			}

			math::Vector3 BoxCollider::getCenter()
			{
				return m_center;
			}

			void BoxCollider::SetSize(math::Vector3 size)
			{
				m_size = size;
			}

			math::Vector3 BoxCollider::getSize()
			{
				return m_size;
			}

		}
	}
}