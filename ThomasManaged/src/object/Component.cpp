#include "stdafx.h"
#include "Component.h"

#include "GameObject.h"

ThomasEditor::GameObject^ ThomasEditor::Component::gameObject::get()
{
	return gcnew ThomasEditor::GameObject(nativePtr->m_gameObject);
}