#include "Rigidbody.h"
#include "Collider.h"
void ThomasEditor::Rigidbody::Awake()
{
	Collider^ collider = gameObject->GetComponent<Collider^>();
	if (collider)
	{
		collider->attachedRigidbody = this;
		((thomas::object::component::Rigidbody*)nativePtr)->setCollisionShape(((thomas::object::component::Collider*)collider->nativePtr)->GetCollisionShape());
	}
}