#include "RigidBodyComponent.h"

#include "../GameObject.h"
#include "../../utils/Math.h"

namespace thomas
{
	namespace object
	{
		namespace component
		{
			void RigidBodyComponent::UpdateRigidbodyMass(float mass)
			{
				btVector3 inertia;
				getCollisionShape()->calculateLocalInertia(mass, inertia);
				setMassProps(mass, inertia);
				updateInertiaTensor();
				
			}
			RigidBodyComponent::RigidBodyComponent() : Component("RigidBodyComponent"), btRigidBody(1, NULL, NULL)
			{
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
				Physics::s_world->addRigidBody(this);
			}

			void RigidBodyComponent::OnDisable()
			{
				Physics::s_world->removeRigidBody(this);
			}

			void RigidBodyComponent::Start()
			{
				Physics::s_world->removeRigidBody(this);
				btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(*(btQuaternion*)&m_gameObject->m_transform->GetRotation(), *(btVector3*)&m_gameObject->m_transform->GetPosition()));
				btCollisionShape* collider = new btBoxShape(btVector3(1, 1, 1));
				btVector3 inertia(0, 0, 0);
				collider->calculateLocalInertia(1, inertia);
				setMotionState(motionState);
				setCollisionShape(collider);
				setMassProps(1, inertia);
				Physics::s_world->addRigidBody(this);
				m_kinematic = false;
			}

			void RigidBodyComponent::Update()
			{
				if (m_kinematic)
				{
					btTransform trans;
					getMotionState()->getWorldTransform(trans);
					trans.setOrigin(Physics::ToBullet(m_gameObject->m_transform->GetPosition()));
					trans.setRotation(Physics::ToBullet(m_gameObject->m_transform->GetRotation()));
					getMotionState()->setWorldTransform(trans);
					setCenterOfMassTransform(trans);
				}
				else
				{
					//Update our transform to match the rigidbody.
					btTransform trans;
					btDefaultMotionState *myMotionState = (btDefaultMotionState *)getMotionState();
					trans = myMotionState->m_graphicsWorldTrans;
					math::Vector3 pos = (math::Vector3)trans.getOrigin();
					math::Quaternion rot = (math::Quaternion)trans.getRotation();
					m_gameObject->m_transform->SetRotation(rot);
					m_gameObject->m_transform->SetPosition(pos);
				}
				
			}
			void RigidBodyComponent::SetKinematic(bool kinematic)
			{
				
				if (kinematic && !m_kinematic)
				{
					m_mass = -getInvMass();
					m_kinematic = kinematic;
					Physics::s_world->removeRigidBody(this);
					UpdateRigidbodyMass(0);
					Physics::s_world->addRigidBody(this);
				}
				else if(!kinematic && m_kinematic)
				{
					m_kinematic = kinematic;
					Physics::s_world->removeRigidBody(this);
					UpdateRigidbodyMass(m_mass);
					Physics::s_world->addRigidBody(this);
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
				UpdateRigidbodyMass(m_mass);
				Physics::s_world->addRigidBody(this);
			}
			void RigidBodyComponent::SetMass(float mass)
			{
				Physics::s_world->removeRigidBody(this);
				m_mass = mass;
				UpdateRigidbodyMass(m_mass);
				Physics::s_world->addRigidBody(this);
			}
			float RigidBodyComponent::GetMass()
			{
				return m_mass;
			}
		}
	}
}

