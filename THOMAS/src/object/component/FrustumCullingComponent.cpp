#include "FrustumCullingComponent.h"
#include "../GameObject.h"
#include "Transform.h"
namespace thomas
{
	namespace object
	{
		namespace component
		{
			FrustumCullingComponent::FrustumCullingComponent() : Component("FrustumCullingComponent")
			{

			}
			void FrustumCullingComponent::Start()
			{
				m_radius = 0.0f;
				m_position = math::Vector3(0.0f, 0.0f, 0.0f);
			}

			void FrustumCullingComponent::Update()
			{
				SetPosition(m_gameObject->m_transform->GetPosition());
			}

			void FrustumCullingComponent::SetRadius(float radius)
			{
				m_radius = radius;
			}

			void FrustumCullingComponent::SetPosition(math::Vector3 position)
			{
				m_position = position;
			}

			float FrustumCullingComponent::GetRadius()
			{
				return m_radius;
			}

			math::Vector3 thomas::object::component::FrustumCullingComponent::GetPosition()
			{
				return m_position;
			}
		}
	}
}