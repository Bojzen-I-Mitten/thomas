#include "BoxCollider.h"
#include "../../../editor/gizmos/Gizmos.h"
#include "../../GameObject.h"
namespace thomas
{
	namespace object
	{
		namespace component
		{
			BoxCollider::BoxCollider() : Collider(new btBoxShape(btVector3(.5f,.5f,.5f)))
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

			void BoxCollider::OnDrawGizmosSelected()
			{
				editor::Gizmos::SetColor(math::Color(0, 1, 0));
				editor::Gizmos::SetMatrix(m_gameObject->m_transform->GetWorldMatrix());
				editor::Gizmos::DrawBoundingOrientedBox(DirectX::BoundingOrientedBox(m_center, m_size*0.5f, math::Quaternion::Identity));
			}

		}
	}
}