#include "Collider.h"
#include "Rigidbody.h"
void ThomasEngine::Collider::attachedRigidbody::set(Rigidbody^ value)
{
	m_attachedRigidbody = value;
	if (m_attachedRigidbody)
	{
		((thomas::object::component::Collider*)nativePtr)->SetAttachedRigidbody((thomas::object::component::Rigidbody*)value->nativePtr);
	}
	else
	{
		((thomas::object::component::Collider*)nativePtr)->SetAttachedRigidbody(nullptr);
	}
}