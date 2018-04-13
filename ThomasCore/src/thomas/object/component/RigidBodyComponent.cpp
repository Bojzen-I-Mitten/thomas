#include "RigidBodyComponent.h"

#include "../GameObject.h"
#include "../../utils/Math.h"
#include "Transform.h"

namespace thomas
{
	namespace object
	{
		namespace component
		{
			void RigidBodyComponent::UpdateRigidbodyMass()
			{
				float mass = m_kinematic ? 0 : m_mass;
				btVector3 inertia;
				getCollisionShape()->calculateLocalInertia(mass, inertia);
				setMassProps(mass, inertia);
				updateInertiaTensor();
				
			}
			RigidBodyComponent::RigidBodyComponent() : btRigidBody(1, NULL, NULL)
			{
				Physics::s_world->removeRigidBody(this);
				btDefaultMotionState* motionState = new btDefaultMotionState();
				setMotionState(motionState);
				m_kinematic = false;
				m_mass = 1;
			}
			
			RigidBodyComponent::~RigidBodyComponent()
			{
				delete getMotionState();
				Physics::s_world->removeCollisionObject(this);
				delete getCollisionShape();
				
				Physics::s_world->removeRigidBody(this);
				
			}

			void RigidBodyComponent::OnEnable()
			{
				btTransform trans;
				trans.setFromOpenGLMatrix(*m_gameObject->m_transform->GetWorldMatrix().m);
				getMotionState()->setWorldTransform(trans);
				setCenterOfMassTransform(trans);
				btCollisionShape* collider = new btBoxShape(btVector3(1, 1, 1));
				setCollisionShape(collider);
				UpdateRigidbodyMass();
				Physics::s_world->addRigidBody(this);
			}

			void RigidBodyComponent::OnDisable()
			{
				Physics::s_world->removeRigidBody(this);
			}

			void RigidBodyComponent::OnDestroy()
			{
				OnDisable();
			}




			void RigidBodyComponent::Update()
			{
				if (m_kinematic)
				{
					btTransform trans;
					trans.setFromOpenGLMatrix(*m_gameObject->m_transform->GetWorldMatrix().m);
					getMotionState()->setWorldTransform(trans);
					setCenterOfMassTransform(trans);
				}
				else
				{
					//Update our transform to match the rigidbody.
					btTransform trans;
					float* transMatrix = *math::Matrix().m;
					getMotionState()->getWorldTransform(trans);
					trans.getOpenGLMatrix(transMatrix);
					math::Matrix newMatrix = math::Matrix::CreateScale(m_gameObject->m_transform->GetScale()) * math::Matrix(transMatrix);
					m_gameObject->m_transform->SetLocalMatrix(newMatrix);
				}
				
			}
			void RigidBodyComponent::SetKinematic(bool kinematic)
			{
				
				if (kinematic != m_kinematic)
				{
					m_kinematic = kinematic;
					if (initialized)
					{
						Physics::s_world->removeRigidBody(this);
						UpdateRigidbodyMass();
						Physics::s_world->addRigidBody(this);
					}
					
				}
		
			}
			bool RigidBodyComponent::IsKinematic()
			{
				return m_kinematic;
			}
			void RigidBodyComponent::SetCollider(btCollisionShape * collider)
			{
				Physics::s_world->removeRigidBody(this);
				delete getCollisionShape();
				setCollisionShape(collider);
				UpdateRigidbodyMass();
				Physics::s_world->addRigidBody(this);
			}
			void RigidBodyComponent::SetMass(float mass)
			{
				m_mass = mass;
				if (initialized)
				{
					Physics::s_world->removeRigidBody(this);
					UpdateRigidbodyMass();
					Physics::s_world->addRigidBody(this);
					
				}
					
				
			}
			float RigidBodyComponent::GetMass()
			{
				return m_mass;
			}
		}
	}
}

