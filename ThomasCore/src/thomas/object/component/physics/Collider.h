#pragma once

#include "../Component.h"
#include "../../../Physics.h"
namespace thomas
{
	namespace object
	{
		namespace component
		{
			class Rigidbody;
			class Collider : public Component
			{
			public:
				Collider(btCollisionShape* collisionShape);
				~Collider();
				
				Rigidbody* GetAttachedRigidbody();
				void SetAttachedRigidbody(Rigidbody* rb);
				bool IsTrigger();
				void SetTrigger(bool trigger);

				void Awake();
				void OnEnable();
				void OnDisable();
				void OnDestroy();

				btCollisionShape* GetCollisionShape();
				
			protected:
				bool m_trigger = false;
				btCollisionShape* m_collisionShape = nullptr;
				btCollisionObject* m_collisionObject = nullptr;
				Rigidbody* m_attachedRigidbody = nullptr;
			};
		}
	}
}