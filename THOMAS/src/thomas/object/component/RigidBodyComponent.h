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
				void UpdateRigidbodyMass(float mass);
			public:
				RigidBodyComponent();
				~RigidBodyComponent();
				void OnEnable();
				void OnDisable();
				void Update();
				void SetKinematic(bool kinematic);
				bool IsKinematic();
				void SetCollider(btCollisionShape* collider);
				void SetMass(float mass);
				float GetMass();
			private:
				float m_mass;
				bool m_kinematic;
			};
		}
	}
}