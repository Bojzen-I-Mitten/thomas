#include "Collider.h"

namespace thomas
{
	namespace object
	{
		namespace component
		{
			Collider::Collider(btCollisionShape * collisionShape)
			{
				m_collisionShape = collisionShape;
			}

			Collider::~Collider()
			{
				
			}

			Rigidbody * Collider::GetAttachedRigidbody()
			{
				return m_attachedRigidbody;
			}

			void Collider::SetAttachedRigidbody(Rigidbody * rb)
			{
				m_attachedRigidbody = rb;
			}

			bool Collider::IsTrigger()
			{
				return m_trigger;
			}

			void Collider::SetTrigger(bool trigger)
			{
				m_trigger = trigger;
			}

			void Collider::Awake()
			{
			}

			void Collider::OnEnable() //maybe later
			{
				if (m_attachedRigidbody == nullptr) //only created if no rigidbody
				{
					m_collisionObject = new btCollisionObject();
					m_collisionObject->setCollisionShape(m_collisionShape);
					Physics::s_world->addCollisionObject(m_collisionObject);
				}
			}

			void Collider::OnDisable()
			{
			}

			void Collider::OnDestroy()
			{
				//SAFE_DELETE(m_collisionShape);
			}

			btCollisionShape * Collider::GetCollisionShape()
			{
				return m_collisionShape;
			}

		}
	}
}

