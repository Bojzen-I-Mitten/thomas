#include "Rigidbody.h"
#include "Collider.h"
void ThomasEngine::Rigidbody::Awake()
{
	Collider^ collider = gameObject->GetComponent<Collider^>();
	if (collider)
	{
		collider->attachedRigidbody = this;
		((thomas::object::component::Rigidbody*)nativePtr)->setCollisionShape(((thomas::object::component::Collider*)collider->nativePtr)->GetCollisionShape());
	}
}

void ThomasEngine::Rigidbody::OnDestroy()
{
	List<Collider^>^ colliders = gameObject->GetComponents<Collider^>();
	for each(Collider^ c in colliders)
	{
		c->attachedRigidbody = nullptr;
	}
	Component::OnDestroy();
}