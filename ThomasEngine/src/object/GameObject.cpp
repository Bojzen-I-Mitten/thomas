
#include "GameObject.h"
#include "..\ThomasManaged.h"

#include "component\physics\BoxCollider.h"
#include "component\physics\SphereCollider.h"

void ThomasEditor::GameObject::Destroy()
{
	Monitor::Enter(Scene::CurrentScene->GetGameObjectsLock());
	Monitor::Enter(m_componentsLock);
	for (int i = 0; i < m_components.Count; i++) {
		m_components[i]->Destroy();
		i--;
	}
	Object::Destroy();
	m_components.Clear();
	Monitor::Exit(m_componentsLock);

	Scene::CurrentScene->GameObjects->Remove(this);
	ThomasWrapper::SelectedGameObjects->Remove(this);
	Monitor::Exit(Scene::CurrentScene->GetGameObjectsLock());
}

ThomasEditor::GameObject ^ ThomasEditor::GameObject::CreatePrimitive(PrimitiveType type)
{

	GameObject^ gameObject = gcnew GameObject("new" + type.ToString());
	gameObject->AddComponent<RenderComponent^>()->model = Model::GetPrimitive(type);

	switch (type)
	{
	case PrimitiveType::Cube:
		gameObject->AddComponent<BoxCollider^>();
		break;
	case PrimitiveType::Sphere:
		gameObject->AddComponent<SphereCollider^>();
		break;
	default:
		break;
	}

	return gameObject;
	
}
