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
				Physics::RemoveRigidBody(this);
				btDefaultMotionState* motionState = new btDefaultMotionState();
				setMotionState(motionState);
				m_kinematic = false;
				m_mass = 1;
			}
			
			RigidBodyComponent::~RigidBodyComponent()
			{
				Physics::RemoveRigidBody(this);
				delete getMotionState();
				
				Physics::s_world->removeCollisionObject(this);
				delete getCollisionShape();				
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
				Physics::AddRigidBody(this);

			}

			void RigidBodyComponent::OnDisable()
			{
				Physics::RemoveRigidBody(this);				
			}

			void RigidBodyComponent::OnDestroy()
			{
				OnDisable();
			}

			void RigidBodyComponent::UpdateRigidbodyToTransform()
			{			
				btTransform trans;
				float* transMatrix = *math::Matrix().m;
				getMotionState()->getWorldTransform(trans);
				trans.getOpenGLMatrix(transMatrix);
				math::Matrix newMatrix = math::Matrix::CreateScale(m_gameObject->m_transform->GetScale()) * math::Matrix(transMatrix);
				m_gameObject->m_transform->SetLocalMatrix(newMatrix);
				m_prevMatrix = newMatrix;
				m_gameObject->m_transform->SetDirty(true);
			}

			void RigidBodyComponent::UpdateTransformToRigidBody()
			{
				math::Matrix currentMatrix = m_gameObject->m_transform->GetWorldMatrix();
				if (m_prevMatrix != currentMatrix) //maybe use internal bool in transform
				{
					btTransform trans;
					trans.setFromOpenGLMatrix(*m_gameObject->m_transform->GetWorldMatrix().m);
					getMotionState()->setWorldTransform(trans);
					setCenterOfMassTransform(trans);
				}
			}

			void RigidBodyComponent::SetKinematic(bool kinematic)
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

			bool RigidBodyComponent::IsKinematic()
			{
				return m_kinematic;
			}

			void RigidBodyComponent::SetCollider(btCollisionShape * collider)
			{
				Physics::RemoveRigidBody(this);
				delete getCollisionShape();
				setCollisionShape(collider);
				UpdateRigidbodyMass();
				Physics::AddRigidBody(this);
			}

			void RigidBodyComponent::SetMass(float mass)
			{
				m_mass = mass;
				if (initialized)
				{
					Physics::RemoveRigidBody(this);
					UpdateRigidbodyMass();
					Physics::AddRigidBody(this);				
				}		
			}

			float RigidBodyComponent::GetMass()
			{
				return m_mass;
			}
		}
	}
}

