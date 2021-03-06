#include "Component.h"
#include "GameObject.h"
#include <thomas\object\GameObject.h>
#include "../ScriptingManager.h"

ThomasEngine::Component::Component() : Object(new thomas::object::component::Component())
{
	setGameObject(GameObject::s_lastObject);
}

ThomasEngine::Component::Component(thomas::object::component::Component * ptr) : Object(ptr)
{
	if(GameObject::s_lastObject)
		setGameObject(GameObject::s_lastObject);
}

ThomasEngine::Transform^ ThomasEngine::Component::transform::get()
{
	return gameObject->transform;
}

void ThomasEngine::Component::setGameObject(GameObject ^ gObj)
{
	m_gameObject = gObj;
	
	((thomas::object::component::Component*)nativePtr)->m_gameObject = (thomas::object::GameObject*)gObj->nativePtr;
	((thomas::object::GameObject*)m_gameObject->nativePtr)->m_components.push_back(((thomas::object::component::Component*)nativePtr));
}

void ThomasEngine::Component::Destroy()
{
	Monitor::Enter(m_gameObject->m_componentsLock);
	OnDestroy();
	for (int i = 0; i < ((thomas::object::GameObject*)m_gameObject->nativePtr)->m_components.size(); i++)
	{
		auto component = ((thomas::object::GameObject*)m_gameObject->nativePtr)->m_components[i];
		if (component == nativePtr)
		{
			((thomas::object::GameObject*)m_gameObject->nativePtr)->
				m_components.erase(((thomas::object::GameObject*)m_gameObject->nativePtr)->m_components.begin() + i);
			break;
		}
	}
	

	m_gameObject->Components->Remove(this);
	thomas::object::Object::Destroy(nativePtr);
	Monitor::Exit(m_gameObject->m_componentsLock);
}

List<Type^>^ ThomasEngine::Component::GetAllComponentTypes()
{
	List<System::Type^>^ types = gcnew List<System::Type^>(System::Reflection::Assembly::GetAssembly(Scene::typeid)->GetExportedTypes());
	

	Assembly^ scriptAssembly = ScriptingManger::GetAssembly();
	if (scriptAssembly)
		types->AddRange(scriptAssembly->GetExportedTypes());

	if (editorAssembly)
		types->AddRange(editorAssembly->GetExportedTypes());

	for (int i = 0; i < types->Count; i++)
	{
		if (!Component::typeid->IsAssignableFrom(types[i]))
		{
			types->RemoveAt(i);
			i--;
		}
	}
	return types;
}

List<Type^>^ ThomasEngine::Component::GetAllAddableComponentTypes()
{
	List<System::Type^>^ types = GetAllComponentTypes();

	for (int i = 0; i < types->Count; i++)
	{
		Type^ t = types[i];
		if (t->IsDefined(HideInInspector::typeid, false))
		{
			types->RemoveAt(i);
			i--;
		}
	}
	return types;
}
