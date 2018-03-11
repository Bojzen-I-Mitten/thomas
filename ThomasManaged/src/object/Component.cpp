
#include "Component.h"

#include "GameObject.h"
#include <thomas\object\GameObject.h>


ThomasEditor::Component::Component() : Object(new thomas::object::component::Component())
{
	setGameObject(GameObject::s_lastObject);
}

ThomasEditor::Component::Component(thomas::object::component::Component * ptr) : Object(ptr)
{
	if(GameObject::s_lastObject)
		setGameObject(GameObject::s_lastObject);
}

void ThomasEditor::Component::setGameObject(GameObject ^ gObj)
{
	m_gameObject = gObj;
	
	((thomas::object::component::Component*)nativePtr)->m_gameObject = (thomas::object::GameObject*)gObj->nativePtr;
	((thomas::object::GameObject*)m_gameObject->nativePtr)->m_components.push_back(((thomas::object::component::Component*)nativePtr));
}

void ThomasEditor::Component::Destroy()
{
	gameObject->Components->Remove(this);
	thomas::object::Object::Destroy(nativePtr);
}
