#pragma once
#include "Component.h"
#include "../../Physics.h"
namespace thomas
{
	namespace object
	{
		namespace component
		{
			class THOMAS_API RigidBodyComponent : public Component, public btRigidBody
			{
			public:
				struct Collision
				{
					RigidBodyComponent* thisRigidbody;
					RigidBodyComponent* otherRigidbody;
				};
			private:
				void UpdateRigidbodyMass();
			public:
				RigidBodyComponent();
				~RigidBodyComponent();
				void OnEnable();
				void OnDisable();
				void OnDestroy();
				void UpdateRigidbodyToTransform();
				void UpdateTransformToRigidBody();
				void SetKinematic(bool kinematic);
				bool IsKinematic();
				void SetCollider(btCollisionShape* collider);
				void SetMass(float mass);
				float GetMass();
			private:
				math::Matrix m_prevMatrix;
				float m_mass;
				bool m_kinematic;
			};
		}
	}
}