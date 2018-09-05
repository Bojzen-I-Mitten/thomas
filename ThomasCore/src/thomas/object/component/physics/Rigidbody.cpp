#include "Rigidbody.h"

#include "../../GameObject.h"
#include "../../../utils/Math.h"
#include "../Transform.h"
#include "Collider.h"

namespace thomas
{
	namespace object
	{
		namespace component
		{
			void Rigidbody::UpdateRigidbodyMass()
			{
				float mass = m_kinematic ? 0 : m_mass;
				btVector3 inertia = getLocalInertia();
				if (getCollisionShape())
				{
					getCollisionShape()->calculateLocalInertia(mass, inertia);
					
				}
				setMassProps(mass, inertia);
				updateInertiaTensor();
				
			}
			Rigidbody::Rigidbody() : btRigidBody(1, NULL, NULL)
			{
				Physics::RemoveRigidBody(this);
				btDefaultMotionState* motionState = new btDefaultMotionState();
				setMotionState(motionState);
				m_kinematic = false;
				m_mass = 1;
			}
			
			Rigidbody::~Rigidbody()
			{
				Physics::RemoveRigidBody(this);
				delete getMotionState();
				
				Physics::s_world->removeCollisionObject(this);
				delete getCollisionShape();				
			}
			void Rigidbody::OnEnable()
			{
				btTransform trans;
				trans.setFromOpenGLMatrix(*m_gameObject->m_transform->GetWorldMatrix().m);
				getMotionState()->setWorldTransform(trans);
				setCenterOfMassTransform(trans);
				UpdateRigidbodyMass();
				Physics::AddRigidBody(this);

			}


			void Rigidbody::OnDisable()
			{
				Physics::RemoveRigidBody(this);				
			}

			void Rigidbody::OnDestroy()
			{
				OnDisable();
			}

			void Rigidbody::UpdateRigidbodyToTransform()
			{			
				btTransform trans;
				getMotionState()->getWorldTransform(trans);
				m_gameObject->m_transform->SetPosition((math::Vector3)trans.getOrigin());
				m_gameObject->m_transform->SetRotation((math::Quaternion)trans.getRotation());
				m_gameObject->m_transform->SetDirty(true);

				m_prevMatrix = m_gameObject->m_transform->GetWorldMatrix();
			}

			void Rigidbody::UpdateTransformToRigidBody()
			{
				if (m_prevMatrix != m_gameObject->m_transform->GetWorldMatrix())
				{
					btTransform trans;
					trans.setOrigin((btVector3&)m_gameObject->m_transform->GetPosition());
					trans.setRotation((btQuaternion&)m_gameObject->m_transform->GetRotation());
					getMotionState()->setWorldTransform(trans);
					setCenterOfMassTransform(trans);
					Physics::s_world->updateSingleAabb(this);
					activate();
				}
				
			}

			void Rigidbody::SetKinematic(bool kinematic)
			{
				
				if (kinematic != m_kinematic)
				{
					m_kinematic = kinematic;
					if (initialized)
					{
						Physics::RemoveRigidBody(this);
						UpdateRigidbodyMass();
						Physics::AddRigidBody(this);
					}
					
				}	
			}
			bool Rigidbody::IsKinematic()
			{
				return m_kinematic;
			}
			void Rigidbody::SetCollider(btCollisionShape * collider)
			{
				Physics::RemoveRigidBody(this);
				delete getCollisionShape();
				setCollisionShape(collider);
				UpdateRigidbodyMass();
				Physics::AddRigidBody(this);
			}
			void Rigidbody::SetMass(float mass)
			{
				m_mass = mass;
				if (initialized)
				{
					Physics::RemoveRigidBody(this);
					UpdateRigidbodyMass();
					Physics::AddRigidBody(this);
					
				}
			}
			float Rigidbody::GetMass()
			{
				return m_mass;
			}
		}
	}
}

