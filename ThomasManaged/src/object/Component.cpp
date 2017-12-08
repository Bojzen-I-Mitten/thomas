#include "stdafx.h"
#include "Component.h"

#include "GameObject.h"
#include <thomas\object\GameObject.h>


void ThomasEditor::Component::setGameObject(GameObject ^ gObj)
{
	m_gameObject = gObj;
	
	((thomas::object::component::Component*)nativePtr)->m_gameObject = (thomas::object::GameObject*)gObj->nativePtr;
	((thomas::object::GameObject*)m_gameObject->nativePtr)->m_components.push_back(((thomas::object::component::Component*)nativePtr));
}
