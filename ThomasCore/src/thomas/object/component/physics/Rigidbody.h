#pragma once
#include "../Component.h"
#include "../../../Physics.h"
namespace thomas
{
	namespace object
	{
		namespace component
		{
			class Collider;
			class THOMAS_API Rigidbody : public Component, public btRigidBody
			{
			public:
				struct Collision
				{
					Rigidbody* thisRigidbody;
					Rigidbody* otherRigidbody;
				};
			private:
				void UpdateRigidbodyMass();
			public:
				Rigidbody();
				~Rigidbody();
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