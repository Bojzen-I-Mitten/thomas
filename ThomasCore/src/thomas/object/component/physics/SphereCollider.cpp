#include "SphereCollider.h"
#include "../../GameObject.h"
#include "../../../editor/gizmos/Gizmos.h"
namespace thomas
{
	namespace object
	{
		namespace component
		{
			SphereCollider::SphereCollider() : Collider(new btSphereShape(0.5f))
			{
				m_center = math::Vector3(0, 0, 0);
				m_radius = 0.5f;
			}

			void SphereCollider::SetCenter(math::Vector3 center)
			{
				m_center = center;
			}

			math::Vector3 SphereCollider::getCenter()
			{
				return m_center;
			}

			void SphereCollider::SetRadius(float size)
			{
				m_radius = size;
				((btSphereShape*)m_collisionShape)->setUnscaledRadius(m_radius);
				
			}

			float SphereCollider::GetRadius()
			{
				return m_radius;
			}

			void SphereCollider::OnDrawGizmosSelected()
			{

				editor::Gizmos::SetColor(math::Color(0, 1, 0));
				editor::Gizmos::SetMatrix(m_gameObject->m_transform->GetWorldMatrix());
				editor::Gizmos::DrawBoundingSphere(DirectX::BoundingSphere(m_center, m_radius));
			}

		}
	}
}
